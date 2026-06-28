# TinyVM

> *A virtual computer built from the ground up.*

TinyVM is an experimental virtual machine that evolves into a complete software ecosystem with its own CPU architecture, instruction set, assembler, kernel, runtime libraries, and operating environment.

The goal isn't to emulate an existing computer.

The goal is to build one.

---

## Vision

Most virtual machines are designed to execute existing languages.

TinyVM takes the opposite approach.

Everything is built specifically for this machine.

* Custom CPU Architecture
* Custom Assembly Language
* Custom Executable Format
* Custom Kernel
* Custom System Libraries
* Custom Shell
* Custom Toolchain

Every layer is designed together.

---

## Project Structure

```text
TinyVM/
├── vm/                # CPU emulator
├── kernel/            # Kernel
├── shell/             # Command shell
├── assembler/         # Assembler
├── sdk/               # System libraries
├── include/           # Public headers
├── docs/              # Documentation
├── examples/          # Sample programs
└── tools/             # Utilities
```

---

## Features

* Virtual CPU
* Custom instruction set
* Memory management
* Registers
* Stack
* Interrupt system
* System call interface
* Kernel API
* Command shell
* Executable loader
* Custom assembler
* Modular architecture

---

## Philosophy

TinyVM is built around one simple idea:

> **Own the entire stack.**

Instead of relying on existing operating systems, instruction sets, executable formats, or development environments, TinyVM defines everything itself.

That means every design decision—from registers to syscalls—is intentional.

---

## Current Progress

* [x] CPU Emulator
* [x] Kernel
* [x] Basic Shell
* [ ] System Calls
* [ ] Assembler
* [ ] Executable Loader
* [ ] Runtime Library
* [ ] File System
* [ ] Debugger
* [ ] Package Manager

---

## Building

```bash
git clone https://github.com/not-flick/tiny_vm.git

cd tiny_vm

cd Build

cmake ..
cmake --build .
```

---

## Example

```asm
section .text

start:
    mov r0, message
    call print

    halt

section .data

message:
    db "Hello, TinyVM!", 0
```

---

## Roadmap

### Phase 1

* CPU
* Memory
* Instructions

### Phase 2

* Kernel
* Syscalls
* Shell

### Phase 3

* Assembler
* Executable format
* Libraries

### Phase 4

* Debugger
* Optimizations
* Standard SDK

### Phase 5

* Self-hosting toolchain (long-term goal)

---

## Why?

Because building a computer from scratch—even a virtual one—is one of the best ways to understand how computers actually work.

TinyVM is as much a learning project as it is a systems programming project.

---

## License

Apache 2.0
