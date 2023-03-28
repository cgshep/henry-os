# Henry OS

Henry OS is a basic X86 operating system built from first principles for research and educational purposes.

It provides a more sophisticated and useful 'bare bones' implementation for prototyping advanced OS concepts and ideas.


<img src="henryos_screenshot.png" alt="Henry OS screenshot" width="45%" height="45%" />

## Features

+ Integrates GNU GRUB / multiboot for reaching (32-bit) protected mode from real mode.
+ Builds the OS into an ISO that works in QEMU.
+ GDT and IDT implementation in X86 ASM and C respectively.
+ A scrolling terminal implementation with keyboard input.
+ Functional software interrupts, e.g. use `int` in X86 ASM.
+ Hardware IRQs: see `keyboard.c`
+ A mini C stdlib.
+ A built-in reverse Polish notation calculator via `calc`
+ Works in QEMU.

## Usage

1. Clone the repo.
2. Get [i686-elf-tools](https://github.com/lordmilko/i686-elf-tools) up and running.
3. Run `make` to build Henry OS, then `make run` to run it in QEMU
4. Make your own changes and have fun!

Note that the project has only been tested using QEMU.

## Credits

+ The [OSDev.org](https://osdev.org) community.
+ The [OpenBSD](www.openbsd.org) project.