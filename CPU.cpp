#include "inc/CPU.h"

CPU::CPU() {
    a = 0;
    x = 0;
    y = 0;
    p = 0;
    pc = 0;
    sp = 0;

    std::fill(memory, memory + MEMORY_SIZE, 0);
}
CPU::~CPU() {
}