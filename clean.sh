#!/bin/sh

set -e
. ./config.sh

for PROJECT in $PROJECT; do
    (cd $PROJECT && $MAKE clean)
done

rm -rf sysroot
rm -rf isodir
rm -rf myos.iso