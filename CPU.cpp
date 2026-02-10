#include "inc/CPU.h"

CPU::CPU(SharedData *shared_data) {
    a = 0;
    x = 0;
    y = 0;
    p = 0;
    s = 0;
    pc = 0;

    this->shared_data = shared_data;
}
CPU::~CPU() {
}

void CPU::init() {
    s = 0xfd;

    std::fill(memory, memory + MEMORY_SIZE, 0);

    set_flag(I);
}
void CPU::reset() {
    s -= 3;
    pc = (memory[0xfffc] | (memory[0xfffd] << 8));
    set_flag(I);
}

void CPU::run() {
    while(shared_data->get_is_running()) {
        ;
    }
}

void CPU::stop() {
    shared_data->set_is_running(false);
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