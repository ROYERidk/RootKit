#!/usr/bin/env bash

sudo qemu-system-x86_64 -kernel ./linux/arch/x86/boot/bzImage -initrd ./initramfs.cpio.gz -append "console=ttyS0" -nographic
