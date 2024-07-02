#
# Copyright (c) - All Rights Reserved.
# 
# See the LICENCE file for more information.
#

qemu-system-x86_64 -bios /usr/share/OVMF/OVMF_CODE.fd -m 128 -drive file=$1/image.img,format=raw -debugcon stdio -global isa-debugcon.iobase=0xe9