#!/usr/bin/env bash

# - Build Alpine disk image with minimal installation
# - Copy kernel source to disk image
# - Configure GRUB bootloader
# - Automount next stage disk image

set -euo pipefail

# General paths
WORKDIR="./vm"

# Disk configuration
DISK_IMG="$WORKDIR/disk.img"
LOOP_DEVICE=""

# Check if the vm folder already exists
#if [ ! -d "$WORKDIR" ]; then
#    echo "[+] VM folder doesn't exist, creating it"
#    mkdir -p "$WORKDIR"
#fi

echo "[+] Creating disk image..."
truncate -s 450M "$DISK_IMG"

echo "[+] Creating partition table..."
/sbin/parted -s "$DISK_IMG" mktable msdos
/sbin/parted -s "$DISK_IMG" mkpart primary ext4 1 "100%"
/sbin/parted -s "$DISK_IMG" set 1 boot on

echo "[+] Setting up loop device..."
sudo losetup -Pf "$DISK_IMG"
LOOP_DEVICE=$(losetup -l | grep -vi "deleted" | grep "$DISK_IMG" | awk '{print $1}')
if [ -z "$LOOP_DEVICE" ]; then
    echo "[-] Loop device not found"
    exit 1
fi

echo "[+] Formatting partition as ext4..."
sudo mkfs.ext4 "${LOOP_DEVICE}p1"

echo "[+] Mounting partition..."
mkdir -p vm/my-rootfs
sudo mount "${LOOP_DEVICE}p1" vm/my-rootfs
sudo chown -R "$USER:$USER" vm/my-rootfs

echo "[+] Installing minimal Alpine Linux..."
docker run -it --rm --volume /home/dan/Documents/alpine_rootkit/vm/my-rootfs:/my-rootfs alpine sh -c '
    apk add openrc util-linux build-base;
    ln -s agetty /etc/init.d/agetty.ttyS0
    echo ttyS0 > /etc/securetty
    rc-update add agetty.ttyS0 default
    rc-update add root default
    echo "root:" | chpasswd;
    rc-update add devfs boot;
    rc-update add procfs boot;
    rc-update add sysfs boot;
    for d in bin etc lib root sbin usr; do tar c "/$d" | tar x -C /my-rootfs; done;
    for dir in dev proc run sys var; do mkdir -p /my-rootfs/${dir}; done
'

echo "[+] Copying Kernel source to rootfs..."
mkdir -p vm/my-rootfs/boot 
sudo cp linux/arch/x86/boot/bzImage vm/my-rootfs/boot/vmlinuz

echo "[+] Configuring GRUB..."
sudo mkdir -p vm/my-rootfs/boot/grub

sudo bash -c 'cat <<EOF > "./vm/my-rootfs/boot/grub/grub.cfg"
set default=0
set timeout=0
serial
terminal_input serial
terminal_output serial
set root=(hd0,1)
menuentry "ldk-kit" {
    linux /boot/vmlinuz root=/dev/sda1 console=ttyS0 noapic quiet
}
EOF'

echo "[+] Installing GRUB in /home/dan/Documents/alpine_rootkit/vm/my-rootfs/boot through ${LOOP_DEVICE}p1"
sudo grub-install --directory=/usr/lib/grub/i386-pc --boot-directory=/home/dan/Documents/alpine_rootkit/vm/my-rootfs/boot "$LOOP_DEVICE"

echo "[+] Cleaning up..."
sudo umount vm/my-rootfs
sudo losetup -d "$LOOP_DEVICE"

echo "[*] Disk image created successfully at /home/dan/Documents/alpine_rootkit/"$DISK_IMG""