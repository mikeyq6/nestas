#include "inc/CPU.h"

CPU::CPU(SharedData *shared_data) {
    a = 0;
    x = 0;
    y = 0;
    p = 0;
    s = 0;
    pc = 0;

    this->shared_data = shared_data;

    ppu = new PPU();
}
CPU::~CPU() {
    delete ppu;
}

void CPU::init() {
    s = 0xfd;

    std::fill(memory, memory + MEMORY_SIZE, 0);

    set_flag(I);

    ppu->init();
}
void CPU::reset() {
    s -= 3;
    pc = (memory[0xfffc] | (memory[0xfffd] << 8));
    set_flag(I);
}

void CPU::run() {
    uint8_t cur_inst = 0;
    Instruction inst;
    while(shared_data->get_is_running()) {
        // fetch
        get_next_instruction(&cur_inst);

        // decode
        decode_instruction(cur_inst, &inst);

        // execute

        // check interrupts
    }
}

void CPU::get_next_instruction(uint8_t *inst) {
    *inst = memory[pc++];
}

void CPU::decode_instruction(uint8_t cur_inst, Instruction *inst) {
    switch(cur_inst) {
        case 0x00: // BRK
            inst->opcode = 0x00;
            inst->cycles = 7;
            break;
        default:
            inst->opcode = cur_inst;
            inst->cycles = 2; // default cycle count for unknown instructions
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

// stack operations
void CPU::push(uint8_t value) {
    memory[0x100 + s] = value;
    s--;
}
uint8_t CPU::pull() {
    s++;
    return memory[0x100 + s];
}