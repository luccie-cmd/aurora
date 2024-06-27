#!/bin/bash
set -e

# Variables
IMG="bin/image.img"
MNT_DIR="mnt"
KERNEL_CC="g++ -Ikernel/libc -m64 -mcmodel=large -Ikernel -Wall -Werror -Wextra -O2 -ffreestanding -nostdlib -mno-red-zone -I /usr/include -std=c++20 -c"
KERNEL_LD="g++ -m64 -mcmodel=large -O2 -nostdlib -ffreestanding -std=c++20 -Wl,-T kernel/linker.ld -Wl,-Map=./bin/kernel.map"
KERNEL_ASM="nasm -felf64 -Ikernel"


# Step 0: Make folders and remove old ones
rm -rf bin
mkdir -p bin
mkdir -p bin/uefi
mkdir -p bin/kernel

# Step 1: Build limine bootloader
cp kernel/boot/BOOTX64.EFI bin/BOOTX64.EFI
cp kernel/boot/limine.cfg bin/limine.cfg

# Step 2: Compile and link all kernel files
compile_recursive() {
    local source_dir="$1"
    local target_dir="$2"

    # Loop through each file in the source directory
    for file in "$source_dir"/*; do
        if [ -d "$file" ]; then
            # If it's a directory, recursively compile files inside it
            local subdir=$(basename "$file")
            compile_recursive "$source_dir/$subdir" "$target_dir/$subdir"
        elif [ "${file##*.}" = "cc" ]; then
            # If it's a .cc file, compile it
            local relative_path="${file#$source_dir/}"
            local target_file="$target_dir/${relative_path%.c}.c.o"

            # Create necessary directories in target path
            mkdir -p "$(dirname "$target_file")"

            # Compile the file
            $KERNEL_CC "$file" -o "$target_file"
            echo "Compiled: $file -> $target_file"
        elif [ "${file##*.}" = "asm" ]; then
            local relative_path="${file#$source_dir/}"
            local target_file="$target_dir/${relative_path%.asm}.asm.o"
            
            # Create necessary directories in target path
            mkdir -p "$(dirname "$target_file")"

            # Compile the file
            $KERNEL_ASM "$file" -o "$target_file"
            echo "Compiled: $file -> $target_file"
        fi
    done
}
compile_recursive "./kernel" "./bin/kernel"
# Function to find .o files recursively
find_object_files() {
    local source_dir="$1"
    local object_files=()

    # Find all .o files recursively under source_dir and store in object_files array
    while IFS= read -r -d '' file; do
        object_files+=("$file")
    done < <(find "$source_dir" -type f -name '*.o' -print0)

    # Print all found .o files
    printf '%s\n' "${object_files[@]}"
}

# Starting point: directory containing .o files (adjust as needed)
object_dir="./bin/kernel"

# Find all .o files recursively under ./bin/kernel
object_files=$(find_object_files "$object_dir")

# Link the .o files using the linker command
echo $KERNEL_LD $object_files -o ./bin/kernel.aur
$KERNEL_LD $object_files -o ./bin/kernel.aur

if [ $1 == "compile" ]; then
    echo "User specified only compile so exit"
    exit 0
fi


# Step 3: Create an EFI System Partition and add the UEFI application
# Remove old image if exists
rm -f $IMG

# Create a 64MB disk image
dd if=/dev/zero of=$IMG bs=1M count=128

# Create a partition table
parted $IMG --script mklabel gpt
parted $IMG --script mkpart EFI FAT32 2048s 100%

# Set the partition as an ESP
parted $IMG --script set 1 boot on

# Create a loopback device for the image
LOOP_DEVICE=$(sudo losetup --show -f -P $IMG)

# Create a filesystem in the partition
sudo mkfs.fat ${LOOP_DEVICE}p1

# Create a mount point and mount the partition
mkdir -p $MNT_DIR
sudo mount ${LOOP_DEVICE}p1 $MNT_DIR

# Create EFI directory structure and copy the UEFI application
sudo mkdir -p $MNT_DIR/EFI/BOOT
sudo mkdir -p $MNT_DIR/boot
sudo cp bin/BOOTX64.EFI $MNT_DIR/EFI/BOOT
sudo cp bin/limine.cfg $MNT_DIR/boot
sudo cp bin/kernel.aur $MNT_DIR/boot
sudo cp -r image/* $MNT_DIR

# Unmount the partition and detach the loopback device
sudo umount $MNT_DIR
sudo losetup -d $LOOP_DEVICE

# Clean up mount directory
rmdir $MNT_DIR

# Step 4: Boot the image with QEMU
qemu-system-x86_64 -bios /usr/share/OVMF/OVMF_CODE.fd -m 128 -drive file=$IMG,format=raw -debugcon stdio -global isa-debugcon.iobase=0xe9