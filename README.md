# TinyVM

> *A virtual computer built from scratch.*

TinyVM is an educational project that aims to build an entire computing stack from the ground up—from a custom shell to a virtual CPU, assembler, compiler, and eventually a complete operating system environment.

The goal isn't to recreate Windows or Linux. The goal is to understand how computers actually work by building every layer ourselves.

---

## Vision

Most people write software **for** an operating system.

TinyVM is about writing the operating system itself.

Starting with nothing more than C++ and a terminal window, TinyVM gradually evolves into its own computing environment.

Eventually it will include:

* A custom shell
* A virtual filesystem
* A kernel
* A virtual CPU
* A custom instruction set
* An assembler
* An executable format
* A compiler
* A standard library
* A package manager
* (Maybe) a graphical desktop

Every component is designed to be implemented from scratch to maximize learning.

---

## Current Features

### Bootloader

* Creates the TinyVM virtual disk
* Initializes directory structure
* Creates user directories
* Generates configuration files
* Launches TinyShell

### TinyShell

* Interactive shell
* Linux-inspired prompt

```text
username@tinyvm:~$
```

* Modular command system
* Command parser
* Filesystem navigation
* Config loading
* Unicode terminal support

### File System

Current virtual disk layout:

```text
C:\
└── tiny_vm
    ├── c
    │   └── users
    │       └── <username>
    ├── system
    │   └── config.txt
    ├── boot
    ├── bin
    ├── temp
    └── logs
```

---

## Current Commands

```
help
clear
exit

pwd
cd
ls

mkdir
touch
rm
rmdir

cat
echo

whoami
hostname
version
```

---

## Roadmap

### Phase 1

* ✅ Bootloader
* ✅ TinyShell
* ✅ Command parser
* ✅ Virtual filesystem abstraction
* ✅ Modular command architecture

---

### Phase 2

* Command history
* Tab completion
* Environment variables
* Aliases
* Better terminal interface

---

### Phase 3

* Kernel API
* Process management
* Memory abstraction
* Filesystem abstraction

---

### Phase 4

* TinyVM Instruction Set Architecture (ISA)
* Virtual CPU
* Registers
* Memory
* Stack
* Interrupts
* System calls

---

### Phase 5

* TinyASM
* Linker
* Executable loader
* TinyVM executable format

---

### Phase 6

* Tiny Language
* Compiler
* Standard library

---

### Phase 7

* User-space applications
* Package manager
* Networking
* GUI (experimental)

---

## Project Structure

```text
TinyVM/
│
├── TinyVM/          # Bootloader
├── TinyShell/       # Interactive shell
├── Build/
├── Docs/
└── CMakeLists.txt
```

TinyShell internally is organized as:

```text
TinyShell/
│
├── Commands/
├── parser.cpp
├── filesystem.cpp
├── shell.cpp
└── main.cpp
```

---

## Building

```bash
git clone <repository>

cd TinyVM

cmake -B Build

cmake --build Build
```

Run the bootloader:

```bash
Build/TinyVM.exe
```

---

## Why?

Because understanding computers is far more rewarding than simply using them.

TinyVM is an attempt to answer questions like:

* How does a shell work?
* How does a filesystem work?
* What does a CPU actually execute?
* How does an assembler translate instructions?
* How do kernels communicate with programs?
* What really happens when a program runs?

Rather than reading about these concepts, TinyVM builds them.

---

## Philosophy

TinyVM values simplicity, readability, and learning over performance.

Every subsystem is implemented incrementally, with the goal of making the internals understandable rather than optimized.

The project is intended to grow layer by layer:

```
Shell
↓

Filesystem

↓

Kernel API

↓

Virtual CPU

↓

Assembler

↓

Compiler

↓

Operating System
```

---

## Contributing

Suggestions, bug reports, and pull requests are welcome.

The project is still in its early stages, so the architecture will continue to evolve.

---

## License

Apache License 2.0

---

> "The best way to understand a computer is to build one."
