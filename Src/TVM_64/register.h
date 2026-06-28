#include <cstdint>

enum class Opcode : uint8_t {
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    r8,
    r9,
    r10,
    r11,
    r12,

    sp,  // Stack Pointer
    bp,   // Base Pointer
    ip,   // Instruction Pointer

    flags
};