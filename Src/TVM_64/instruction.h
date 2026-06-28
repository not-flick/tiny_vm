#include <cstdint>

enum class Opcode : uint8_t
{
    // System
    nop,
    syscall,
    halt,

    // Data Movement
    mov,
    push,
    pop,

    // Arithmetic
    add,
    sub,
    mul,
    div,
    rem,

    // Comparison
    cmp,

    // Control Flow
    jmp,
    jz,
    je,
    jne,
    jg,
    jl,
    jge,
    jle,
    call,
    ret
};