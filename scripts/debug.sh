#!/bin/bash

QEMU_ARGS="-S -gdb stdio -m 128"

if [ "$#" -le 1 ]; then
    echo "Usage: ./debug.sh <image_type> <image>"
    exit 1
fi

case "$1" in
    "floppy")   QEMU_ARGS="${QEMU_ARGS} -fda $PWD/$2"
    ;;
    "disk")     QEMU_ARGS="${QEMU_ARGS} -hda $PWD/$2"
    ;;
    *)          echo "Unknown image type $1."
                exit 2
esac

# b *0x7c00
# layout asm
cat > .vscode/.gdb_script.gdb << EOF
    set disassembly-flavor intel
    target remote | qemu-system-x86_64 -bios /usr/share/OVMF/OVMF_CODE.fd  $QEMU_ARGS
EOF

gdb -x .vscode/.gdb_script.gdb
