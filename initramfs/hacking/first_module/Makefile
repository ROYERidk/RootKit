obj-m := w3.o
CC = gcc -Wall
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
EXTRA_CFLAGS += -I /usr/lib/modules/$(uname -r)/build/include

all:
		make -C ../../../linux/ M=$(PWD) modules

clean:
		$(MAKE) -C $(KDIR) M=$(PWD) clean
