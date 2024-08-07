#!/bin/python3
#
# Copyright (c) - All Rights Reserved.
# 
# See the LICENCE file for more information.
#


import os
import glob
import sys
import subprocess
from script.util import parseSize, compareFiles

def readConfig(path: str) -> dict[str, list[str]]:
    """Reads a configuration file and returns a dictionary of lists of strings."""
    config = {}
    try:
        with open(path, "r") as f:
            lines = f.readlines()
            for line in lines:
                line = line.strip()
                if ':' in line:
                    key, value = line.split(":", 1)  # Split at the first ':'
                    key = key.strip()
                    value = value.strip().strip("'")  # Strip leading/trailing whitespace and single quotes
                    config[key] = [v.strip().strip("'") for v in value.split(",")]  # Split by ',' and strip quotes from each part
    except FileNotFoundError:
        print(f"Error: Configuration file '{path}' not found.")
        return config
    return config

    
def writeConfig(config: dict[str, list[str]], path: str) -> None:
    """Writes a configuration dictionary to a file."""
    with open(path, "w") as f:
        for key, values in config.items():
            f.write(f"{key}:")
            for val in enumerate(values):
                f.write(f"'{val[1]}'")
                if val[0]+1 < len(values):
                    # pass
                    f.write(", ")
            f.write('\n')

def checkConfig(config: dict[str, list[str]], allowed_config: list[list[str, list[str], bool]]) -> bool:
    """Checks if a configuration dictionary is valid."""
    for key, values in config.items():
        print(key, values)
        for allowed_key, allowed_values, required in allowed_config:
            if key == allowed_key:
                if len(allowed_values) == 0:
                    continue
                if required and len(values) == 0:
                    return False
                if not required and len(values) > 0:
                    return False
                for val in values:
                    if val not in allowed_values:
                        return False
    return True

CONFIG = readConfig("./script/config.py")
OLD_CONFIG = readConfig("./script/config.py.old")
ALLOWED_CONFIG = [
    ["config", ["release", "debug"], True],
    ["arch", ["x86", "x64"], True],
    ["compiler", ["gcc", "clang"], True],
    ["imageFS", ["fat32"], True],
    ["bootloader", ["limine-uefi"], True],
    ["outDir", [], True],
    ["imageSize", [], False],
    ["debug", ["yes", "no"], True],
    ["mt19937", ["yes", "no"], True]
]
if not checkConfig(CONFIG, ALLOWED_CONFIG):
    print("Invalid config file.")
    print("Allowed config items")
    for option in ALLOWED_CONFIG:
        name = option[0]
        values = option[1]
        required = option[2]
        print(f"{name} (required = {required})")
        if len(values) == 0:
            print("    This can be anything as long as it's provided")
        else:
            for val in values:
                print(f"  - {val}")
    exit(1)
writeConfig(CONFIG, "./script/config.py.old")
force_rebuild = False
if OLD_CONFIG != CONFIG:
    force_rebuild = True
    print("Configuration changed, rebuilding...")
# Add some default values to the config
CONFIG["CFLAGS"] = ['-c', '-ffreestanding', '-finline-functions']
# if CONFIG["compiler"][0] == "clang":
CONFIG["CFLAGS"] += ["-fmax-errors=1"]
CONFIG["ASFLAGS"] = ['-felf64']
CONFIG["LDFLAGS"] = ['-nostdlib', '-ffreestanding']
if "imageSize" not in CONFIG:
    CONFIG["imageSize"] = '128m'

if "yes" in CONFIG.get("debug"):
    CONFIG["CFLAGS"] += ["-O0"]
    CONFIG["CFLAGS"] += ["-g"]
    CONFIG["CFLAGS"] += ["-DDEBUG"]
else:
    CONFIG["CFLAGS"] += ["-O2"]
    CONFIG["CFLAGS"] += ["-DNDEBUG"]
if "yes" in CONFIG.get("mt19937"):
    CONFIG["CFLAGS"] += ["-DMT19937"]
if "x64" in CONFIG.get("arch"):
    CONFIG["CFLAGS"] += ["-m64"]
    CONFIG["CFLAGS"] += ["-mcmodel=large"]
elif "x86" in CONFIG.get("arch"):
    CONFIG["CFLAGS"] += ["-m32"]

if "yes" in CONFIG.get("debug"):
    CONFIG["LDFLAGS"] += ["-O0"]
    CONFIG["LDFLAGS"] += ["-g"]
else:
    CONFIG["LDFLAGS"] += ["-O2"]
if "x64" in CONFIG.get("arch"):
    CONFIG["LDFLAGS"] += ["-m64"]
    CONFIG["LDFLAGS"] += ["-mcmodel=large"]
elif "x86" in CONFIG.get("arch"):
    CONFIG["LDFLAGS"] += ["-m32"]

def callCmd(command):
    # Run the command and capture the output
    result = subprocess.run(command, capture_output=True, text=True, shell=True)
    if result.returncode != 0:
        print(result.stderr)
        exit(1)
    return result.stdout

def checkExtension(file: str, valid_extensions: list[str]):
    for ext in valid_extensions:
        if file.endswith(ext):
            return True
    return False

def getExtension(file):
    return file.split(".")[-1]

def buildC(file):
    compiler = CONFIG["compiler"][0]
    options = CONFIG["CFLAGS"]
    command = compiler + " " + file
    for option in options:
        command += " " + option
    print(f"C     {file}")
    command += f" -o {CONFIG['outDir'][0]}/{file}.o"
    callCmd(command)

def buildASM(file):
    compiler = "nasm"
    options = CONFIG["ASFLAGS"]
    command = compiler + " " + file
    for option in options:
        command += " " + option
    print(f"AS    {file}")
    command += f" -o {CONFIG['outDir'][0]}/{file}.o"
    callCmd(command)

def buildKernel(kernel_dir: str):
    files = glob.glob(kernel_dir+'/**', recursive=True)
    CONFIG["CFLAGS"] += [f"-I{kernel_dir}"]
    CONFIG["CFLAGS"] += [f"-Ilibc"]
    for file in files:
        if not os.path.isfile(file):
            continue
        if not checkExtension(file, ["c", "asm"]):
            continue
        basename = os.path.basename(os.path.dirname(os.path.realpath(__file__)))
        callCmd(f"cpp -I{kernel_dir} -Ilibc {file} -o ./tmp.txt")
        if not force_rebuild and compareFiles("./tmp.txt", os.path.abspath(f"/tmp/{basename}/cache/{file}")):
            continue
        callCmd(f"mkdir -p {CONFIG['outDir'][0]}/{os.path.dirname(file)}")
        callCmd(f"mkdir -p /tmp/{basename}/cache/{os.path.dirname(file)}")
        # with open("./tmp.txt", "w") as f:
        callCmd(f"cpp -I{kernel_dir} -Ilibc {file} -o ./tmp.txt")
        callCmd(f"cp ./tmp.txt /tmp/{basename}/cache/{file}")
        if getExtension(file) == "c":
            buildC(file)
        elif getExtension(file) == "asm":
            buildASM(file)

def linkKernel(kernel_dir, linker_file, libc_file):
    files = glob.glob(kernel_dir+'/**', recursive=True)
    command = CONFIG["compiler"][0]
    options = CONFIG["LDFLAGS"]
    for option in options:
        command += " " + option
    for file in files:
        if not os.path.isfile(file):
            continue
        if not checkExtension(file, ["o"]):
            continue
        command += " " + file
    command += f" -Wl,-T {linker_file}"
    command += f" {libc_file}"
    if CONFIG["debug"][0] == "yes":
        command += f" -Wl,-Map={CONFIG['outDir'][0]}/kernel.map"
    command += f" -o {CONFIG['outDir'][0]}/kernel.aur"
    callCmd(command)

def makeImageFile(out_file):
    size = parseSize(CONFIG["imageSize"][0])
    command = f"dd if=/dev/zero of={out_file} bs=1M count={size//parseSize("1M")}"
    callCmd(command)

def makePartitionTable(out_file):
    print("> Making GPT partition")
    command = f"parted {out_file} --script mklabel gpt"
    callCmd(command)
    print("> Making EFI partition")
    command = f"parted {out_file} --script mkpart EFI {CONFIG['imageFS'][0]} 2048s 100%"
    callCmd(command)
    print("> Setting EFI partition to be bootable")
    command = f"parted {out_file} --script set 1 boot on"
    callCmd(command)

def setupLoopDevice(out_file):
    print("> Setting up loop device")
    command = f"sudo losetup --show -f -P {out_file} > /tmp/tmp.txt"
    callCmd(command)
    loop_device = ""
    with open("/tmp/tmp.txt") as f:
        loop_device = f.readline()
    loop_device = loop_device.strip()
    print(f"> Loop device: {loop_device}")
    return loop_device

def makeFileSystem(loop_device):
    callCmd(f"sudo mkfs.fat {loop_device}p1")

def mountFs(device, boot, kernel):
    callCmd(f"mkdir -p mnt")
    callCmd(f"sudo mount {device}p1 mnt")
    callCmd(f"sudo mkdir -p mnt/EFI/BOOT")
    callCmd(f"sudo mkdir -p mnt/boot")
    callCmd(f"sudo cp {boot} mnt/EFI/BOOT")
    callCmd(f"sudo cp {kernel} mnt/boot")
    callCmd(f"sudo cp {CONFIG['outDir'][0]}/syscall.tbl mnt/boot")
    if "limine-uefi" in CONFIG["bootloader"]:
        callCmd(f"sudo cp {CONFIG['outDir'][0]}/limine.cfg mnt/boot")
    callCmd(f"sudo cp -r image/* mnt")
    callCmd(f"sudo umount mnt")
    callCmd(f"sudo losetup -d {device}")
    callCmd(f"rm -rf mnt")


def buildImage(out_file, boot_file, kernel_file):
    callCmd(f"rm -f {out_file}")
    makeImageFile(out_file)
    makePartitionTable(out_file)
    LOOP_DEVICE=setupLoopDevice(out_file)
    makeFileSystem(LOOP_DEVICE)
    mountFs(LOOP_DEVICE, boot_file, kernel_file)

def buildLibc(directory, out_file):
    CONFIG["CFLAGS"] += [f'-I{directory}']
    os.makedirs(CONFIG["outDir"][0]+'/'+directory, exist_ok=True)
    files = glob.glob(f"{directory}/**", recursive=True)
    for file in files:
        if not os.path.isfile(file):
            continue
        if not checkExtension(file, ["c", "asm"]):
            continue
        basename = os.path.basename(os.path.dirname(os.path.realpath(__file__)))
        if not force_rebuild and compareFiles(os.path.abspath(file), os.path.abspath(f"/tmp/{basename}/cache/{file}")):
            continue
        callCmd(f"mkdir -p {CONFIG['outDir'][0]}/{os.path.dirname(file)}")
        callCmd(f"mkdir -p /tmp/{basename}/cache/{os.path.dirname(file)}")
        callCmd(f"cp {file} /tmp/{basename}/cache/{file}")
        if getExtension(file) == "c":
            buildC(file)
        elif getExtension(file) == "asm":
            buildASM(file)
    
    files = glob.glob(f"{CONFIG['outDir'][0]}/{directory}/**", recursive=True)
    obj_files = []
    for file in files:
        if not os.path.isfile(file):
            continue
        if not checkExtension(file, ["o"]):
            continue
        obj_files.append(file)
    obj_files_str = " ".join(obj_files)
    cmd = f"ar rcs {out_file} {obj_files_str}"
    callCmd(cmd)

def main():
    basename = os.path.basename(os.path.dirname(os.path.realpath(__file__)))
    if len(sys.argv) > 1:
        if sys.argv[1] == "clean":
            callCmd(f"rm -rf /tmp/{basename}")
            callCmd(f"rm -rf {CONFIG['outDir'][0]}")
    if force_rebuild:
        print("Rebuilding...")
    print("> Creating necesarry dirs")
    callCmd(f"mkdir -p {CONFIG['outDir'][0]}")
    callCmd(f"mkdir -p {CONFIG['outDir'][0]}/kernel")
    if 'limine-uefi' in CONFIG["bootloader"]:
        print("> Copying limine stuff")
        callCmd(f"cp kernel/boot/BOOTX64.EFI {CONFIG['outDir'][0]}")
        callCmd(f"cp kernel/boot/limine.cfg {CONFIG['outDir'][0]}")
    else:
        print("TODO: Other bootloaders")
        exit(1)
    callCmd(f"cp kernel/boot/syscall.tbl {CONFIG['outDir'][0]}")
    print("> Building LibC")
    buildLibc("libc", f"{CONFIG['outDir'][0]}/libc.a")
    print("> Building kernel")
    buildKernel("kernel")
    print("> Linking kernel")
    linkKernel(f"{CONFIG['outDir'][0]}/kernel", "kernel/linker.ld", f"{CONFIG['outDir'][0]}/libc.a")
    if len(sys.argv) > 1:
        if sys.argv[1] == "compile":
            return
    buildImage(f"{CONFIG['outDir'][0]}/image.img", f"{CONFIG['outDir'][0]}/BOOTX64.EFI", f"{CONFIG['outDir'][0]}/kernel.aur")
    if len(sys.argv) > 1:
        if sys.argv[1] == "run":
            callCmd(f"./script/run.sh {CONFIG['outDir'][0]}")

if __name__ == '__main__':
    main()