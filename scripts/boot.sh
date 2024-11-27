#!/usr/bin/env bash

# Stage 0
# - Start VM

WORKDIR="./vm"
DISK="$WORKDIR/disk.img"
share_folder="/tmp/qemu-share"

echo "[+] Starting VM..."
qemu-system-x86_64 -enable-kvm \
    -nographic \
    -m 2048 -smp 2 \
    -drive file="$DISK",format=raw \
    -fsdev local,security_model=passthrough,id=fsdev0,path=/tmp/qemu-share \
    -device virtio-9p-pci,fsdev=fsdev0,mount_tag=share

#/usr/bin/qemu-system-x86_64 \
#    -nographic \
#    -enable-kvm \
#    -m 2G \
#    -drive file="$DISK",format=raw \
#    -nic user,model=rtl8139 \
#    -virtfs local,path=$share_folder,mount_tag=host0,security_model=passthrough,id=foobar