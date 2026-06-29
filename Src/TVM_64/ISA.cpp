#include "ISA.h"

#include "instruction.h"
#include "kernal.h"
#include "syscall.h"

#include <algorithm>
#include <limits>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace
{
    constexpr int kRegisterCount = 17;

    inline int ClampRegister(int reg)
    {
        return std::clamp(reg, 0, kRegisterCount - 1);
    }

    inline uint64_t& RegisterRef(CPU& cpu, int reg)
    {
        return cpu.regs[ClampRegister(reg)];
    }

    inline std::unordered_map<const CPU*, std::vector<uint64_t>>& StackStorage()
    {
        static std::unordered_map<const CPU*, std::vector<uint64_t>> stackStorage;
        return stackStorage;
    }

    inline std::unordered_map<uint64_t, uint64_t>& MemoryStorage()
    {
        static std::unordered_map<uint64_t, uint64_t> memoryStorage;
        return memoryStorage;
    }

    inline void UpdateFlags(CPU& cpu, uint64_t result)
    {
        cpu.regs[static_cast<int>(Register::flags)] = result == 0 ? 0 : 1;
    }

    inline std::string ToString(const Value& value)
    {
        return std::visit([](const auto& v) -> std::string
        {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, std::string>)
            {
                return v;
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                return v ? "true" : "false";
            }
            else
            {
                return std::to_string(v);
            }
        }, value);
    }
}

void syscall(TinyKernel& kernel, CPU& cpu, int id, const Args& args)
{
    (void)cpu;

    std::vector<std::string> kernelArgs;
    kernelArgs.reserve(args.size());

    for (const auto& value : args)
    {
        kernelArgs.push_back(ToString(value));
    }

    SyscallDispatcher dispatcher(kernel);
    dispatcher.Handle(static_cast<SyscallID>(id), kernelArgs);
}

void mov(CPU& cpu, int dst, uint64_t value)
{
    RegisterRef(cpu, dst) = value;
}

void push(CPU& cpu, int reg)
{
    auto& stack = StackStorage()[&cpu];
    const auto sp = static_cast<std::size_t>(cpu.regs[static_cast<int>(Register::sp)]);
    if (stack.size() <= sp)
    {
        stack.resize(sp + 1);
    }

    stack[sp] = RegisterRef(cpu, reg);
    cpu.regs[static_cast<int>(Register::sp)] = static_cast<uint64_t>(sp + 1);
}

void pop(CPU& cpu, int reg)
{
    auto& stack = StackStorage()[&cpu];
    auto sp = static_cast<std::size_t>(cpu.regs[static_cast<int>(Register::sp)]);
    if (sp == 0)
    {
        RegisterRef(cpu, reg) = 0;
        return;
    }

    --sp;
    RegisterRef(cpu, reg) = stack[sp];
    cpu.regs[static_cast<int>(Register::sp)] = static_cast<uint64_t>(sp);
}

void add(CPU& cpu, int dst, int src)
{
    RegisterRef(cpu, dst) += RegisterRef(cpu, src);
    UpdateFlags(cpu, RegisterRef(cpu, dst));
}

void sub(CPU& cpu, int dst, int src)
{
    RegisterRef(cpu, dst) -= RegisterRef(cpu, src);
    UpdateFlags(cpu, RegisterRef(cpu, dst));
}

void mul(CPU& cpu, int dst, int src)
{
    RegisterRef(cpu, dst) *= RegisterRef(cpu, src);
    UpdateFlags(cpu, RegisterRef(cpu, dst));
}

void idiv(CPU& cpu, int dst, int src)
{
    const auto divisor = RegisterRef(cpu, src);
    if (divisor != 0)
    {
        RegisterRef(cpu, dst) /= divisor;
    }
    else
    {
        RegisterRef(cpu, dst) = 0;
    }
    UpdateFlags(cpu, RegisterRef(cpu, dst));
}

void mod(CPU& cpu, int dst, int src)
{
    const auto divisor = RegisterRef(cpu, src);
    if (divisor != 0)
    {
        RegisterRef(cpu, dst) %= divisor;
    }
    else
    {
        RegisterRef(cpu, dst) = 0;
    }
    UpdateFlags(cpu, RegisterRef(cpu, dst));
}

bool cmp(CPU& cpu, int lhs, int rhs)
{
    const auto left = RegisterRef(cpu, lhs);
    const auto right = RegisterRef(cpu, rhs);

    if (left == right)
    {
        cpu.regs[static_cast<int>(Register::flags)] = 0;
        return true;
    }

    cpu.regs[static_cast<int>(Register::flags)] = left < right ? 1 : 2;
    return false;
}

void jmp(CPU& cpu, uint64_t target)
{
    cpu.regs[static_cast<int>(Register::ip)] = target;
}

void jz(CPU& cpu, uint64_t target)
{
    if (cpu.regs[static_cast<int>(Register::flags)] == 0)
    {
        cpu.regs[static_cast<int>(Register::ip)] = target;
    }
}

void je(CPU& cpu, uint64_t target)
{
    if (cpu.regs[static_cast<int>(Register::flags)] == 0)
    {
        cpu.regs[static_cast<int>(Register::ip)] = target;
    }
}

void jne(CPU& cpu, uint64_t target)
{
    if (cpu.regs[static_cast<int>(Register::flags)] != 0)
    {
        cpu.regs[static_cast<int>(Register::ip)] = target;
    }
}

void jg(CPU& cpu, uint64_t target)
{
    if (cpu.regs[static_cast<int>(Register::flags)] == 2)
    {
        cpu.regs[static_cast<int>(Register::ip)] = target;
    }
}

void jl(CPU& cpu, uint64_t target)
{
    if (cpu.regs[static_cast<int>(Register::flags)] == 1)
    {
        cpu.regs[static_cast<int>(Register::ip)] = target;
    }
}

void jge(CPU& cpu, uint64_t target)
{
    if (cpu.regs[static_cast<int>(Register::flags)] == 0 || cpu.regs[static_cast<int>(Register::flags)] == 2)
    {
        cpu.regs[static_cast<int>(Register::ip)] = target;
    }
}

void jle(CPU& cpu, uint64_t target)
{
    if (cpu.regs[static_cast<int>(Register::flags)] == 0 || cpu.regs[static_cast<int>(Register::flags)] == 1)
    {
        cpu.regs[static_cast<int>(Register::ip)] = target;
    }
}

void call(CPU& cpu, uint64_t target)
{
    push(cpu, static_cast<int>(Register::ip));
    cpu.regs[static_cast<int>(Register::ip)] = target;
}

void ret(CPU& cpu)
{
    pop(cpu, static_cast<int>(Register::ip));
}

void nop(CPU& cpu)
{
    (void)cpu;
}

void halt(CPU& cpu)
{
    cpu.regs[static_cast<int>(Register::ip)] = std::numeric_limits<uint64_t>::max();
}

void load(CPU& cpu, int dst, uint64_t address)
{
    RegisterRef(cpu, dst) = MemoryStorage()[address];
}

void store(CPU& cpu, uint64_t address, int src)
{
    MemoryStorage()[address] = RegisterRef(cpu, src);
}

void lea(CPU& cpu, int dst, uint64_t address)
{
    RegisterRef(cpu, dst) = address;
}

