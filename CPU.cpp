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
        execute_instruction(&inst);

        // check interrupts
    }
}

void CPU::get_next_instruction(uint8_t *inst) {
    *inst = memory[pc++];
}

void CPU::decode_instruction(uint8_t cur_inst, Instruction *inst) {
    inst->opcode = 0x00;
    switch(cur_inst) {
        case 0x00: // BRK
            inst->cycles = 7;
            break;
        case 0x69: // ADC Immediate
            inst->operand1 = memory[pc++];
            inst->cycles = 2;
            break;
        case 0x65: // ADC Zero Page
            inst->operand1 = memory[pc++];
            inst->cycles = 3;
            break;
        case 0x75: // ADC Zero Page,X
            inst->operand1 = memory[pc++];
            inst->cycles = 4;
            break;
        case 0x6d: // ADC Absolute
        case 0x7d: // ADC Absolute,X
        case 0x79: // ADC Absolute,Y
            inst->operand1 = memory[pc++];
            inst->operand2 = memory[pc++];
            inst->cycles = 4; // +1 if page crossed
            break;
        case 0x61: // ADC (Indirect,X)
        case 0x71: // ADC (Indirect),Y
            inst->operand1 = memory[pc++];
            inst->cycles = 6;
            break;
        default:
            inst->opcode = cur_inst;
            inst->cycles = 2; // default cycle count for unknown instructions
    }
}

void CPU::execute_instruction(Instruction *inst) {
    uint32_t addr = 0;
    switch(inst->opcode) {
        case 0x00: // BRK
            set_flag(B);
            push((pc >> 8) & 0xff); // push high byte of PC
            push(pc & 0xff);        // push low byte of PC
            push(p);                // push processor status
            set_flag(I);            // disable interrupts
            pc = (memory[0xfffd] | (memory[0xfffe] << 8)); // load interrupt vector
            break;
        case 0x69: // ADC Immediate
            adc(inst->operand1);
            break;
        case 0x65: // ADC Zero Page
            adc(memory[inst->operand1]);
            break;
        case 0x75: // ADC Zero Page,X
            adc(memory[(inst->operand1 + x) & 0xff]);
            break;
        case 0x6d: // ADC Absolute
            adc(memory[inst->operand2 << 8 | inst->operand1]);
            break;
        case 0x7d: // ADC Absolute,X
            if(inst->operand1 + x > 0xff) inst->cycles++; // page crossed
            adc(memory[((inst->operand2 << 8 | inst->operand1) + x) & 0xffff]);
            break;
        case 0x79: // ADC Absolute,Y
            if(inst->operand1 + y > 0xff) inst->cycles++; // page crossed
            adc(memory[((inst->operand2 << 8 | inst->operand1) + y) & 0xffff]);
            break;
        case 0x61:// ADC (Indirect,X)
            addr = (memory[inst->operand1 + 1] << 8 | memory[inst->operand1]) + x;
            adc(memory[addr & 0xffff]);
            break;
        case 0x71:// ADC (Indirect,Y)
            addr = (memory[inst->operand1 + 1] << 8 | memory[inst->operand1]) + y;
            adc(memory[addr & 0xffff]);
            break;
        default:
            // For unknown instructions, we can just ignore them or log an error.
            break;
    }
}    

uint16_t CPU::get_indirect_x_address(uint8_t value) {
    int8_t low = memory[(value + x) & 0xff];
    uint8_t high = memory[(value + x + 1) & 0xff];
    return memory[high + low] * 256;
}
uint16_t CPU::get_indirect_y_address(uint8_t value) {
    uint8_t low = memory[value];
    uint8_t high = memory[value + 1];
    return memory[(high + low) & 0xff] * 256 + y;
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

void CPU::adc(uint8_t value) {
    uint16_t sum = a + value + (is_set(C) ? 1 : 0);
    if(sum > 0xff) set_flag(C); else reset_flag(C);
    if(sum == 0) set_flag(Z); else reset_flag(Z);
    if(sum & 0x80) set_flag(N); else reset_flag(N);
    if (((a ^ value) & 0x80) == 0 && ((a ^ sum) & 0x80) != 0) set_flag(V); else reset_flag(V);   
    a = sum & 0xff;
}