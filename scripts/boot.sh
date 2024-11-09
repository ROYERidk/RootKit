#!/usr/bin/env bash

# Stage 0
# - Start VM

WORKDIR="./vm"
DISK="$WORKDIR/disk.img"

echo "[+] Starting VM..."
/usr/bin/qemu-system-x86_64 \
    -nographic \
    -enable-kvm \
    -m 2G \
    -drive file="$DISK",format=raw \
    -nic user,model=rtl8139