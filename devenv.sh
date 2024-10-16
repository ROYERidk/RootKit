#!/usr/bin/env bash

set -euxo pipefail

# Configure env
export BUILD=$PWD
export INITRAMFS_BUILD=$PWD/initramfs
export BUSYBOX_BUILD=$PWD/busybox

# Create build directory
mkdir -p "$INITRAMFS_BUILD"
pushd "$INITRAMFS_BUILD"
mkdir -p bin sbin etc proc sys dev usr/{s,}bin
popd

# Copy busybox
cp -a "$BUSYBOX_BUILD/_install"/* "$BUILD/initramfs"

# Copy init_loop program, spawing a shell
gcc "$BUILD/init/main.c" -o "$INITRAMFS_BUILD/init_loop"

# Copy basic libc
mkdir -p "$INITRAMFS_BUILD/usr/lib/"
mkdir -p "$INITRAMFS_BUILD/lib64"

cp /usr/lib/libc.so.6 "$INITRAMFS_BUILD/usr/lib/"
cp /usr/lib/libm.so.6 "$INITRAMFS_BUILD/usr/lib/"
cp /usr/lib/libresolv.so.2 "$INITRAMFS_BUILD/usr/lib/"
cp /lib64/ld-linux-x86-64.so.2 "$INITRAMFS_BUILD/lib64"

# Create init program
cat > "$INITRAMFS_BUILD/init" <<END
#!/bin/sh

mount -t proc none /proc
mount -t sysfs none /sys
mount -t devtmpfs none /dev
cat <<!

Boot took $(cut -d' ' -f1 /proc/uptime) seconds
Welcome to your Linux
!
/init_loop
END

chmod +x "$INITRAMFS_BUILD/init"

# Build gzipped CPIO image
pushd "$BUILD/initramfs"
find . -print0 | cpio --null -ov --format=newc | gzip -9 > "$BUILD/initramfs.cpio.gz"
popd
