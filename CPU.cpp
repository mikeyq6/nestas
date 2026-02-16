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
        case 0x29: // AND Immediate
            inst->operand1 = memory[pc++];
            inst->cycles = 2;
            break;
        case 0x65: // ADC Zero Page
        case 0x25: // AND Zero Page
            inst->operand1 = memory[pc++];
            inst->cycles = 3;
            break;
        case 0x75: // ADC Zero Page,X
        case 0x35: // AND Zero Page,X
            inst->operand1 = memory[pc++];
            inst->cycles = 4;
            break;
        case 0x6d: // ADC Absolute
        case 0x7d: // ADC Absolute,X
        case 0x79: // ADC Absolute,Y
        case 0x2d: // AND Absolute
        case 0x3d: // AND Absolute,X
        case 0x39: // AND Absolute,Y
            inst->operand1 = memory[pc++];
            inst->operand2 = memory[pc++];
            inst->cycles = 4; // +1 if page crossed
            break;
        case 0x0e: // ASL Absolute
            inst->operand1 = memory[pc++];
            inst->operand2 = memory[pc++];
            inst->cycles = 6;
            break;
        case 0x1e: // ASL Absolute,X
            inst->operand1 = memory[pc++];
            inst->operand2 = memory[pc++];
            inst->cycles = 7;
            break;
        case 0x61: // ADC (Indirect,X)
        case 0x21: // AND (Indirect,X)
        case 0x16: // ASL (Indirect,X)
            inst->operand1 = memory[pc++];
            inst->cycles = 6;
            break;
        case 0x71: // ADC (Indirect),Y
        case 0x31: // AND (Indirect),Y
        case 0x06: // ASL Zero Page
            inst->operand1 = memory[pc++];
            inst->cycles = 5; // +1 if page crossed
            break;
        case 0xa: // ASL Accumulator
            inst->cycles = 2;
            break;
        case 0x90: // BCC Relative
            inst->operand1 = memory[pc++];
            inst->cycles = 2; // +1 if branch taken, +2 if page crossed
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
            pc = (memory[0xfffe] | (memory[0xffff] << 8)); // load interrupt vector
            break;
        case 0x69: // ADC Immediate
            ADC(inst->operand1);
            break;
        case 0x65: // ADC Zero Page
            ADC(memory[inst->operand1]);
            break;
        case 0x75: // ADC Zero Page,X
            ADC(memory[(inst->operand1 + x) & 0xff]);
            break;
        case 0x6d: // ADC Absolute
            ADC(memory[inst->operand2 << 8 | inst->operand1]);
            break;
        case 0x7d: // ADC Absolute,X
            if(inst->operand1 + x > 0xff) inst->cycles++; // page crossed
            ADC(memory[((inst->operand2 << 8 | inst->operand1) + x) & 0xffff]);
            break;
        case 0x79: // ADC Absolute,Y
            if(inst->operand1 + y > 0xff) inst->cycles++; // page crossed
            ADC(memory[((inst->operand2 << 8 | inst->operand1) + y) & 0xffff]);
            break;
        case 0x61: // ADC (Indirect,X)
            addr = get_indirect_x_address(inst->operand1);
            ADC(memory[addr & 0xffff]);
            break;
        case 0x71: // ADC (Indirect,Y)
            addr = get_indirect_y_address(inst->operand1);
            ADC(memory[addr & 0xffff]);
            break;
        case 0x29: // AND Immediate
            AND(inst->operand1);
            break;
        case 0x25: // AND Zero Page
            AND(memory[inst->operand1]);
            break;
        case 0x35: // AND Zero Page,X
            AND(memory[(inst->operand1 + x) & 0xff]);
            break;
        case 0x2d: // AND Absolute
            AND(memory[inst->operand2 << 8 | inst->operand1]);
            break;
        case 0x3d: // AND Absolute,X
            if(inst->operand1 + x > 0xff) inst->cycles++; // page crossed
            AND(memory[((inst->operand2 << 8 | inst->operand1) + x) & 0xffff]);
            break;
        case 0x39: // AND Absolute,Y
            if(inst->operand1 + y > 0xff) inst->cycles++; // page crossed
            AND(memory[((inst->operand2 << 8 | inst->operand1) + y) & 0xffff]);
            break;
        case 0x21: // AND (Indirect,X)
            addr = get_indirect_x_address(inst->operand1);
            AND(memory[addr & 0xffff]);
            break;
        case 0x31: // AND (Indirect),Y
            addr = get_indirect_y_address(inst->operand1);
            AND(memory[addr & 0xffff]);
            break;
        case 0x0a: // ASL Accumulator
            a = ASL(a);
            break;
        case 0x06: // ASL Zero Page
            memory[inst->operand1] = ASL(memory[inst->operand1]);
            break;
        case 0x16: // ASL Zero Page,X
            memory[(inst->operand1 + x) & 0xff] = ASL(memory[(inst->operand1 + x) & 0xff]);
            break;
        case 0x0e: // ASL Absolute
            addr = inst->operand2 << 8 | inst->operand1;
            memory[addr] = ASL(memory[addr]);
            break;
        case 0x1e: // ASL Absolute,X
            addr = ((inst->operand2 << 8 | inst->operand1) + x) & 0xffff;
            memory[addr] = ASL(memory[addr]);
            break;
        case 0x90: // BCC Relative
            if(!is_set(C)) {
                pc += (int8_t)inst->operand1; // branch offset is signed
                inst->cycles++; // branch taken
                if((pc & 0xff00) != ((pc - (int8_t)inst->operand1) & 0xff00)) {
                    inst->cycles++; // page crossed
                }
            }
            break;
        default:
            // For unknown instructions, we can just ignore them or log an error.
            break;
    }
}    

uint16_t CPU::get_indirect_x_address(uint8_t value) {
    int8_t low = memory[(value + x) & 0xff];
    uint8_t high = memory[(value + x + 1) & 0xff];
    return memory[((high << 8) + low) & 0xffff];
}
uint16_t CPU::get_indirect_y_address(uint8_t value) {
    uint8_t low = memory[value];
    uint8_t high = memory[value + 1];
    return (memory[((high << 8) + low) & 0xffff] + y) & 0xffff;
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

void CPU::ADC(uint8_t value) {
    uint16_t sum = a + value + (is_set(C) ? 1 : 0);
    if(sum > 0xff) set_flag(C); else reset_flag(C);
    if(sum == 0) set_flag(Z); else reset_flag(Z);
    if(sum & 0x80) set_flag(N); else reset_flag(N);
    if (((a ^ value) & 0x80) == 0 && ((a ^ sum) & 0x80) != 0) set_flag(V); else reset_flag(V);   
    a = sum & 0xff;
}

void CPU::AND(uint8_t value) {
    a &= value;
    if(a == 0) set_flag(Z); else reset_flag(Z);
    if(a & 0x80) set_flag(N); else reset_flag(N);
}

uint8_t CPU::ASL(uint8_t value) {
    if(value & 0x80) set_flag(C); else reset_flag(C);
    value <<= 1;
    if(value == 0) set_flag(Z); else reset_flag(Z);
    if(value & 0x80) set_flag(N); else reset_flag(N);
    return 0;
}