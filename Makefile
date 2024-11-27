obj-m := ldk-kit.o
ldkkit-y := src/ldk-kit.c

CC = gcc
ARCH ?= x86_64
CORES = $(shell expr $$(nproc) - 1 )

KDIR ?= $(PWD)/linux
MDIR ?= $(PWD)

THIS_FILE := $(lastword $(MAKEFILE_LIST))

SCRIPTS ?= $(PWD)/scripts
VM ?= $(PWD)/vm
INSTALL_MOD_PATH ?= $(VM)/my-rootfs

default:
	@echo "[+] Compiling module"
	$(MAKE) ARCH=$(ARCH) KCFLAGS=-Wno-error -C $(KDIR) M=$(MDIR) modules

first-time-setup:
	@echo "[+] Configuring kernel"
	$(MAKE)  ARCH=$(ARCH) -C $(KDIR) x86_64_defconfig
	$(MAKE) -f $(THIS_FILE) kernel

kernel:
	@echo "[+] Compiling kernel"
	$(MAKE)  ARCH=$(ARCH) CC=$(CC) -C $(KDIR) -j $(CORES)

clean:
	@echo "[+] Cleaning workspace"
	$(MAKE)  ARCH=$(ARCH) -C $(KDIR) M=$(MDIR) clean
	rm -rf $(MDIR)/target/
	rm -rf $(MDIR)/vm/

disk:
	@if [ ! -d $(VM) ]; then \
		mkdir -p $(VM); \
	fi
	@if [ ! -f $(VM)/disk.img ]; then \
		echo "[-] Disk image doesn't exist"; \
		echo "[+] Creating VM image disk"; \
		$(SCRIPTS)/build_img.sh; \
	else \
		echo "[+] Disk image exists"; \
	fi

vm: disk
	@echo "[+] Launching VM with QEMU"
	$(SCRIPTS)/boot.sh
