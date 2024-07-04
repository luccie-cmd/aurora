# Aurora
An operating system made for learning more about operating systems and the internal workings of it

# Philosophy
Aurora is by no means a perfect operating system, though it has been designed to work on moderen machines and leave a lot of room open for the user space programs

# Requirements
- A modern x86_64 machine with UEFI boot capabilities
- A USB drive with at least 100MB of space
- Any C and ASM compiler (asm compiler must support intel syntax)
- Any python interpreter (must support python 3+)
- Any virtual machine that supports UEFI (qemu get's installed by default)

# Building
0. Run `$ install_deps.sh`  
This will install all the needed dependecies including gcc, qemu, ovmf (uefi boot for qemu), python3, parted and mtools (for mkfs)
1. Run [build.py](build.py)  
this will read the configuration file from [script/config.py](script/config.py) and build a .img file that you can copy and paste into your USB  
NOTE: If your want to install aurora on the main disk run the `install` command when it boots up, this will list all the devices with their sizes.
2. Run the OS  
When examining [build.py](build.py) you may have noticed that i put some arguments like run, clean and compile when you call `$ build.py run` it will call the run command for the OS

# Licence
Aurora is licensed under the MIT licence, see [LICENCE](LICENCE) for more