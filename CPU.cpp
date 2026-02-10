#include "inc/CPU.h"

CPU::CPU() {
    a = 0;
    x = 0;
    y = 0;
    p = 0;
    s = 0;
    pc = 0;
}
CPU::~CPU() {
}

void CPU::init() {
    s = 0xfd;

    std::fill(memory, memory + MEMORY_SIZE, 0);

    set_flag(I);
}

void CPU::set_flag(FLAG flag) {
    p |= flag;
}
void CPU::reset_flag(FLAG flag) {
    p &= ~flag;
}
bool CPU::is_set(FLAG flag) {
    return (p & flag) != 0;
}