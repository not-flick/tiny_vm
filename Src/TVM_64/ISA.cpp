#include "ISA.h"
#include "ISA.h"
#include "instruction.h"


#include "kernal.h"
#include "syscall.h"

void def_insturction() {

    Opcode op;
    while (true) {

        //arithemetic
        if (op == Opcode::add) {
        }
        else if (op == Opcode::sub) {}
        else if (op == Opcode::mul) {}
        else if (op == Opcode::div) {}
        else if (op == Opcode::rem) {}
        else if (op == Opcode::sub) {}

        //system
        else if (op == Opcode::nop) {}
        else if (op == Opcode::syscall) {}
        else if (op == Opcode::halt) {}

        //data movement
        else if (op == Opcode::mov) {}
        else if (op == Opcode::push) {}
        else if (op == Opcode::pop) {}

        //comparision
        else if (op == Opcode::cmp) {}

        //control flow
        else if (op == Opcode::jmp) {}
        else if (op == Opcode::jz ) {}
        else if (op == Opcode::je ) {}
        else if (op == Opcode::jne ) {}
        else if (op == Opcode::jg ) {}
        else if (op == Opcode::jl ) {}
        else if (op == Opcode::jge ) {}
        else if (op == Opcode::jle ) {}
        else if (op == Opcode::call ) {}
        else if (op == Opcode::ret) {}

        else {
            // console.prin("")
        }


    }
}
