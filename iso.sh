#!/bin/sh

set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/cheakyos.kernel isodir/boot/cheakyos.kernel

cat > isodir/boot/grub/grub.cfg << EOF
menuentry "myos" {
	multiboot /boot/cheakyos.kernel
}
EOF
grub-mkrescue -o cheakyos.iso isodir
