#pragma once
#include <vector>
#include <variant>
#include <string>
#include <cstdint>

#include "register.h"
#include "kernal.h"

struct CPU
{
    uint64_t regs[17]{};
};

using Value = std::variant<
    int64_t,
    uint64_t,
    float,
    double,
    bool,
    std::string
>;

using Args = std::vector<Value>;

void syscall(TinyKernel& kernel, CPU& cpu, int id, const Args& args);

// Data movement
void mov(CPU& cpu, int dst, uint64_t value);
void push(CPU& cpu, int reg);
void pop(CPU& cpu, int reg);

// Arithmetic
void add(CPU& cpu, int dst, int src);
void sub(CPU& cpu, int dst, int src);
void mul(CPU& cpu, int dst, int src);
void idiv(CPU& cpu, int dst, int src);
void mod(CPU& cpu, int dst, int src);

// Comparison
bool cmp(CPU& cpu, int lhs, int rhs);

// Control flow
void jmp(CPU& cpu, uint64_t target);
void jz(CPU& cpu, uint64_t target);
void je(CPU& cpu, uint64_t target);
void jne(CPU& cpu, uint64_t target);
void jg(CPU& cpu, uint64_t target);
void jl(CPU& cpu, uint64_t target);
void jge(CPU& cpu, uint64_t target);
void jle(CPU& cpu, uint64_t target);
void call(CPU& cpu, uint64_t target);
void ret(CPU& cpu);

// System
void nop(CPU& cpu);
void halt(CPU& cpu);

// Memory
void load(CPU& cpu, int dst, uint64_t address);
void store(CPU& cpu, uint64_t address, int src);
void lea(CPU& cpu, int dst, uint64_t address);