#include "inc/CPU.h"

CPU::CPU(SharedData *shared_data) {
    a = 0;
    x = 0;
    y = 0;
    p = 0;
    s = 0;
    pc = 0;

    this->shared_data = shared_data;

    ppu = new PPU(shared_data);
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
    pc = (read_memory(0xfffc) | (read_memory(0xfffd) << 8));
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
    *inst = read_memory(pc++);
}

void CPU::decode_instruction(uint8_t cur_inst, Instruction *inst) {
    inst->opcode = 0x00;
    switch(cur_inst) {
        case 0x0a: // ASL Accumulator
        case 0x18: // CLC Implied
        case 0x38: // SEC Implied
        case 0xd8: // CLD Implied
        case 0xf8: // SED Implied
        case 0xb8: // CLV Implied
        case 0x58: // CLI Implied
        case 0xea: // NOP Implied
        case 0xca: // DEX Implied
        case 0x88: // DEY Implied
        case 0xe8: // INX Implied
        case 0xc8: // INY Implied
        case 0x4a: // LSR Accumulator
        case 0x2a: // ROL Accumulator
        case 0x6a: // ROR Accumulator
            inst->cycles = 2;
            break;
        case 0x69: // ADC Immediate
        case 0x29: // AND Immediate
        case 0x90: // BCC Relative
        case 0xb0: // BCS Relative
        case 0xf0: // BEQ Relative
        case 0xd0: // BNE Relative
        case 0x30: // BMI Relative
        case 0x10: // BPL Relative
        case 0x50: // BVC Relative
        case 0x70: // BVS Relative
        case 0xc9: // CMP Immediate
        case 0xe0: // CPX Immediate
        case 0xc0: // CPY Immediate
        case 0x49: // EOR Immediate
        case 0xa9: // LDA Immediate
        case 0xa2: // LDX Immediate
        case 0xa0: // LDY Immediate
        case 0x09: // ORA Immediate
            inst->operand1 = read_memory(pc++);
            inst->cycles = 2; // +1 if branch taken, +2 if page crossed
            break;
        case 0x48: // PHA Implied
        case 0x08: // PHP Implied
            inst->cycles = 3;
            break;
        case 0x65: // ADC Zero Page
        case 0x25: // AND Zero Page
        case 0x24: // BIT Zero Page
        case 0xc5: // CMP Zero Page
        case 0xe4: // CPX Zero Page
        case 0xd4: // CPY Zero Page
        case 0x45: // EOR Zero Page
        case 0xa5: // LDA Zero Page
        case 0xa6: // LDX Zero Page
        case 0xa4: // LDY Zero Page
        case 0x05: // ORA Zero Page
            inst->operand1 = read_memory(pc++);
            inst->cycles = 3;
            break;
        case 0x4c: // JMP Absolute
            inst->operand1 = read_memory(pc++);
            inst->operand2 = read_memory(pc++);
            inst->cycles = 3;
            break;
        case 0x68: // PLA Implied
        case 0x28: // PLP Implied
            inst->cycles = 4;
            break;
        case 0x75: // ADC Zero Page,X
        case 0x35: // AND Zero Page,X
        case 0xd5: // CMP Zero Page,X
        case 0x55: // EOR Zero Page,X
        case 0xb5: // LDA Zero Page,X
        case 0xb6: // LDX Zero Page,Y
        case 0xb4: // LDY Zero Page,X
        case 0x15: // ORA Zero Page,X
            inst->operand1 = read_memory(pc++);
            inst->cycles = 4;
            break;
        case 0x6d: // ADC Absolute
        case 0x7d: // ADC Absolute,X
        case 0x79: // ADC Absolute,Y
        case 0x2d: // AND Absolute
        case 0x3d: // AND Absolute,X
        case 0x39: // AND Absolute,Y
        case 0x2c: // BIT Absolute
        case 0xcd: // CMP Absolute
        case 0xdd: // CMP Absolute,X
        case 0xec: // CPX Absolute
        case 0xcc: // CPY Absolute
        case 0x4d: // EOR Absolute
        case 0x5d: // EOR Absolute,X
        case 0x59: // EOR Absolute,Y
        case 0xad: // LDA Absolute
        case 0xbd: // LDA Absolute,X
        case 0xb9: // LDA Absolute,Y
        case 0xae: // LDX Absolute
        case 0xbe: // LDX Absolute,Y
        case 0xac: // LDY Absolute
        case 0xbc: // LDY Absolute,X
        case 0x0d: // ORA Absolute
        case 0x1d: // ORA Absolute,X
        case 0x19: // ORA Absolute,Y
            inst->operand1 = read_memory(pc++);
            inst->operand2 = read_memory(pc++);
            inst->cycles = 4; // +1 if page crossed
            break;
        case 0x71: // ADC (Indirect),Y
        case 0x31: // AND (Indirect),Y
        case 0x06: // ASL Zero Page
        case 0xd1: // CMP (Indirect),Y
        case 0xc6: // DEC Zero Page
        case 0x51: // EOR (Indirect),Y
        case 0xe6: // INC Zero Page
        case 0xb1: // LDA (Indirect),Y
        case 0x46: // LSR Zero Page
        case 0x11: // ORA (Indirect),Y
        case 0x26: // ROL Zero Page
        case 0x66: // ROR Zero Page
            inst->operand1 = read_memory(pc++);
            inst->cycles = 5; // +1 if page crossed
            break;
        case 0x6c: // JMP Indirect
            inst->operand1 = read_memory(pc++);
            inst->operand2 = read_memory(pc++);
            inst->cycles = 5; // +1 if page crossed
            break;
        case 0x40: // RTI Implied
        case 0x60: // RTS Implied
            inst->cycles = 6;
            break;
        case 0x61: // ADC (Indirect,X)
        case 0x21: // AND (Indirect,X)
        case 0x16: // ASL (Indirect,X)
        case 0xc1: // CMP (Indirect,X)
        case 0xd6: // DEC Zero Page,X
        case 0x41: // EOR (Indirect,X)
        case 0xf6: // INC Zero Page,X
        case 0xa1: // LDA (Indirect,X)
        case 0x56: // LSR (Indirect,X)
        case 0x01: // ORA (Indirect,X)
        case 0x36: // ROL Zero Page,X
        case 0x76: // ROR Zero Page,X
            inst->operand1 = read_memory(pc++);
            inst->cycles = 6;
            break;
        case 0x0e: // ASL Absolute
        case 0xce: // DEC Absolute
        case 0xee: // INC Absolute
        case 0x20: // JSR Absolute
        case 0x4e: // LSR Absolute
        case 0x2e: // ROL Absolute
        case 0x6e: // ROR Absolute
            inst->operand1 = read_memory(pc++);
            inst->operand2 = read_memory(pc++);
            inst->cycles = 6;
            break;
        case 0x00: // BRK
            inst->cycles = 7;
            break;
        case 0x1e: // ASL Absolute,X
        case 0xde: // DEC Absolute,X
        case 0xfe: // INC Absolute,X
        case 0x5e: // LSR Absolute,X
        case 0x3e: // ROL Absolute,X
        case 0x7e: // ROR Absolute,X
            inst->operand1 = read_memory(pc++);
            inst->operand2 = read_memory(pc++);
            inst->cycles = 7;
            break;
        default:
            inst->opcode = cur_inst;
            inst->cycles = 2; // default cycle count for unknown instructions
    }
}

void CPU::execute_instruction(Instruction *inst) {
    uint32_t addr = 0;
    bool page_crossed = false;
    switch(inst->opcode) {
        case 0x00: // BRK
            set_flag(B);
            push((pc >> 8) & 0xff); // push high byte of PC
            push(pc & 0xff);        // push low byte of PC
            push(p);                // push processor status
            set_flag(I);            // disable interrupts
            pc = (read_memory(0xfffe) | (read_memory(0xffff) << 8)); // load interrupt vector
            break;
        case 0x69: // ADC Immediate
            ADC(inst->operand1);
            break;
        case 0x65: // ADC Zero Page
            ADC(read_memory(inst->operand1));
            break;
        case 0x75: // ADC Zero Page,X
            ADC(read_memory((inst->operand1 + x) & 0xff));
            break;
        case 0x6d: // ADC Absolute
            ADC(read_memory(inst->operand2 << 8 | inst->operand1));
            break;
        case 0x7d: // ADC Absolute,X
            if(inst->operand1 + x > 0xff) inst->cycles++; // page crossed
            ADC(read_memory(((inst->operand2 << 8 | inst->operand1) + x) & 0xffff));
            break;
        case 0x79: // ADC Absolute,Y
            if(inst->operand1 + y > 0xff) inst->cycles++; // page crossed
            ADC(read_memory(((inst->operand2 << 8 | inst->operand1) + y) & 0xffff));
            break;
        case 0x61: // ADC (Indirect,X)
            addr = get_indirect_x_address(inst->operand1);
            ADC(read_memory(addr & 0xffff));
            break;
        case 0x71: // ADC (Indirect,Y)
            addr = get_indirect_y_address(inst->operand1, &page_crossed);
            if(page_crossed) inst->cycles++; // page crossed
            ADC(read_memory(addr & 0xffff));
            break;
        case 0x29: // AND Immediate
            AND(inst->operand1);
            break;
        case 0x25: // AND Zero Page
            AND(read_memory(inst->operand1));
            break;
        case 0x35: // AND Zero Page,X
            AND(read_memory((inst->operand1 + x) & 0xff));
            break;
        case 0x2d: // AND Absolute
            AND(read_memory(inst->operand2 << 8 | inst->operand1));
            break;
        case 0x3d: // AND Absolute,X
            if(inst->operand1 + x > 0xff) inst->cycles++; // page crossed
            AND(read_memory(((inst->operand2 << 8 | inst->operand1) + x) & 0xffff));
            break;
        case 0x39: // AND Absolute,Y
            if(inst->operand1 + y > 0xff) inst->cycles++; // page crossed
            AND(read_memory(((inst->operand2 << 8 | inst->operand1) + y) & 0xffff));
            break;
        case 0x21: // AND (Indirect,X)
            addr = get_indirect_x_address(inst->operand1);
            AND(read_memory(addr & 0xffff));
            break;
        case 0x31: // AND (Indirect),Y
            addr = get_indirect_y_address(inst->operand1, &page_crossed);
            if(page_crossed) inst->cycles++; // page crossed
            AND(read_memory(addr & 0xffff));
            break;
        case 0x0a: // ASL Accumulator
            a = ASL(a);
            break;
        case 0x06: // ASL Zero Page
            write_memory(inst->operand1, ASL(read_memory(inst->operand1)));
            break;
        case 0x16: // ASL Zero Page,X
            write_memory((inst->operand1 + x) & 0xff, ASL(read_memory((inst->operand1 + x) & 0xff)));
            break;
        case 0x0e: // ASL Absolute
            addr = inst->operand2 << 8 | inst->operand1;
            write_memory(addr, ASL(read_memory(addr)));
            break;
        case 0x1e: // ASL Absolute,X
            addr = ((inst->operand2 << 8 | inst->operand1) + x) & 0xffff;
            write_memory(addr, ASL(read_memory(addr)));
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
        case 0xb0: // BCS Relative
            if(is_set(C)) {
                pc += (int8_t)inst->operand1; // branch offset is signed
                inst->cycles++; // branch taken
                if((pc & 0xff00) != ((pc - (int8_t)inst->operand1) & 0xff00)) {
                    inst->cycles++; // page crossed
                }
            }
            break;
        case 0xf0: // BEQ Relative
            if(is_set(Z)) {
                pc += (int8_t)inst->operand1; // branch offset is signed
                inst->cycles++; // branch taken
                if((pc & 0xff00) != ((pc - (int8_t)inst->operand1) & 0xff00)) {
                    inst->cycles++; // page crossed
                }
            }
            break;
        case 0xd0: // BNE Relative
            if(!is_set(Z)) {
                pc += (int8_t)inst->operand1; // branch offset is signed
                inst->cycles++; // branch taken
                if((pc & 0xff00) != ((pc - (int8_t)inst->operand1) & 0xff00)) {
                    inst->cycles++; // page crossed
                }
            }
            break;
        case 0x30: // BMI Relative
            if(is_set(N)) {
                pc += (int8_t)inst->operand1; // branch offset is signed
                inst->cycles++; // branch taken
                if((pc & 0xff00) != ((pc - (int8_t)inst->operand1) & 0xff00)) {
                    inst->cycles++; // page crossed
                }
            }
            break;
        case 0x10: // BPL Relative
            if(!is_set(N)) {
                pc += (int8_t)inst->operand1; // branch offset is signed
                inst->cycles++; // branch taken
                if((pc & 0xff00) != ((pc - (int8_t)inst->operand1) & 0xff00)) {
                    inst->cycles++; // page crossed
                }
            }
            break;
        case 0x50: // BVC Relative
            if(!is_set(V)) {
                pc += (int8_t)inst->operand1; // branch offset is signed
                inst->cycles++; // branch taken
                if((pc & 0xff00) != ((pc - (int8_t)inst->operand1) & 0xff00)) {
                    inst->cycles++; // page crossed
                }
            }
            break;
        case 0x70: // BVS Relative
            if(is_set(V)) {
                pc += (int8_t)inst->operand1; // branch offset is signed
                inst->cycles++; // branch taken
                if((pc & 0xff00) != ((pc - (int8_t)inst->operand1) & 0xff00)) {
                    inst->cycles++; // page crossed
                }
            }
            break;
        case 0x24: // BIT Zero Page
            BIT(read_memory(inst->operand1));
            break;
        case 0x2c: // BIT Absolute
            BIT(read_memory(inst->operand2 << 8 | inst->operand1));
            break;
        case 0x18: // CLC Implied
            reset_flag(C);
            break;
        case 0x38: // SEC Implied
            set_flag(C);
            break;
        case 0xd8: // CLD Implied
            reset_flag(D);
            break;
        case 0xf8: // SED Implied
            set_flag(D);
            break;
        case 0xb8: // CLV Implied
            reset_flag(V);
            break;
        case 0x58: // CLI Implied
            reset_flag(I);
            break;
        case 0xc9: // CMP Immediate
            CMP(a, inst->operand1);
            break;
        case 0xe0: // CPX Immediate
            CMP(x, inst->operand1);
            break;
        case 0xc0: // CPY Immediate
            CMP(y, inst->operand1);
            break;
        case 0xc5: // CMP Zero Page
            CMP(a, read_memory(inst->operand1));
            break;
        case 0xe4: // CPX Zero Page
            CMP(x, read_memory(inst->operand1));
            break;
        case 0xd4: // CPY Zero Page
            CMP(y, read_memory(inst->operand1));
            break;
        case 0xd5: // CMP Zero Page,X
            CMP(a, read_memory((inst->operand1 + x) & 0xff));
            break;
        case 0xcd: // CMP Absolute
            CMP(a, read_memory(inst->operand2 << 8 | inst->operand1));
            break;
        case 0xec: // CPX Absolute
            CMP(x, read_memory(inst->operand2 << 8 | inst->operand1));
            break;
        case 0xcc: // CPY Absolute
            CMP(y, read_memory(inst->operand2 << 8 | inst->operand1));
            break;
        case 0xdd: // CMP Absolute,X
            if(inst->operand1 + x > 0xff) inst->cycles++; // page crossed
            CMP(a, read_memory(((inst->operand2 << 8 | inst->operand1) + x) & 0xffff));
            break;
        case 0xd9: // CMP Absolute,Y
            if(inst->operand1 + y > 0xff) inst->cycles++; // page crossed
            CMP(a, read_memory(((inst->operand2 << 8 | inst->operand1) + y) & 0xffff));
            break;
        case 0xc1: // CMP (Indirect,X)
            addr = get_indirect_x_address(inst->operand1);
            CMP(a, read_memory(addr & 0xffff));
        case 0xd1: // CMP (Indirect),Y
            addr = get_indirect_y_address(inst->operand1, &page_crossed);
            if(page_crossed) inst->cycles++; // page crossed
            CMP(a, read_memory(addr & 0xffff));
            break;
        case 0xc6: // DEC Zero Page
            write_memory(inst->operand1, read_memory(inst->operand1) - 1);
            if(read_memory(inst->operand1) == 0) set_flag(Z); else reset_flag(Z);
            if(read_memory(inst->operand1) & 0x80) set_flag(N); else reset_flag(N);
            break;
        case 0xd6: // DEC Zero Page,X
            addr = (inst->operand1 + x) & 0xff;
            write_memory(addr, read_memory(addr) - 1);
            if(read_memory(addr) == 0) set_flag(Z); else reset_flag(Z);
            if(read_memory(addr) & 0x80) set_flag(N); else reset_flag(N);
            break;
        case 0xce: // DEC Absolute
            addr = inst->operand2 << 8 | inst->operand1;
            write_memory(addr, read_memory(addr) - 1);
            if(read_memory(addr) == 0) set_flag(Z); else reset_flag(Z);
            if(read_memory(addr) & 0x80) set_flag(N); else reset_flag(N);
            break;
        case 0xde: // DEC Absolute,X
            addr = ((inst->operand2 << 8 | inst->operand1) + x) & 0xffff;
            write_memory(addr, read_memory(addr) - 1);
            if(read_memory(addr) == 0) set_flag(Z); else reset_flag(Z);
            if(read_memory(addr) & 0x80) set_flag(N); else reset_flag(N);
            break;
        case 0xca: // DEX Implied
            x--;
            if(x == 0) set_flag(Z); else reset_flag(Z);
            if(x & 0x80) set_flag(N); else reset_flag(N);
            break;
        case 0x88: // DEY Implied
            y--;
            if(y == 0) set_flag(Z); else reset_flag(Z);
            if(y & 0x80) set_flag(N); else reset_flag(N);
            break;
        case 0x49: // EOR Immediate
            EOR(inst->operand1);
            break;
        case 0x45: // EOR Zero Page
            EOR(read_memory(inst->operand1));
            break;
        case 0x55: // EOR Zero Page,X
            EOR(read_memory((inst->operand1 + x) & 0xff));
            break;
        case 0x4d: // EOR Absolute
            EOR(read_memory(inst->operand2 << 8 | inst->operand1));
            break;
        case 0x5d: // EOR Absolute,X
            if(inst->operand1 + x > 0xff) inst->cycles++; // page crossed
            EOR(read_memory(((inst->operand2 << 8 | inst->operand1) + x) & 0xffff));
            break;
        case 0x59: // EOR Absolute,Y
            if(inst->operand1 + y > 0xff) inst->cycles++; // page crossed
            EOR(read_memory(((inst->operand2 << 8 | inst->operand1) + y) & 0xffff));
            break;
        case 0x41: // EOR (Indirect,X)
            addr = get_indirect_x_address(inst->operand1);
            EOR(read_memory(addr & 0xffff));
            break;
        case 0x51: // EOR (Indirect),Y
            addr = get_indirect_y_address(inst->operand1, &page_crossed);
            if(page_crossed) inst->cycles++; // page crossed
            EOR(read_memory(addr & 0xffff));
            break;
        case 0xe6: // INC Zero Page
            write_memory(inst->operand1, read_memory(inst->operand1) + 1);
            if(read_memory(inst->operand1) == 0) set_flag(Z); else reset_flag(Z);
            if(read_memory(inst->operand1) & 0x80) set_flag(N); else reset_flag(N);
            break;
        case 0xf6: // INC Zero Page,X
            addr = (inst->operand1 + x) & 0xff;
            write_memory(addr, read_memory(addr) + 1);
            if(read_memory(addr) == 0) set_flag(Z); else reset_flag(Z);
            if(read_memory(addr) & 0x80) set_flag(N); else reset_flag(N);
            break;
        case 0xee: // INC Absolute
            addr = inst->operand2 << 8 | inst->operand1;
            write_memory(addr, read_memory(addr) + 1);
            if(read_memory(addr) == 0) set_flag(Z); else reset_flag(Z);
            if(read_memory(addr) & 0x80) set_flag(N); else reset_flag(N);
            break;
        case 0xfe: // INC Absolute,X
            addr = ((inst->operand2 << 8 | inst->operand1) + x) & 0xffff;
            write_memory(addr, read_memory(addr) + 1);
            if(read_memory(addr) == 0) set_flag(Z); else reset_flag(Z);
            if(read_memory(addr) & 0x80) set_flag(N); else reset_flag(N);
            break;
        case 0xe8: // INX Implied
            x++;
            if(x == 0) set_flag(Z); else reset_flag(Z);
            if(x & 0x80) set_flag(N); else reset_flag(N);
            break;
        case 0xc8: // INY Implied
            y++;
            if(y == 0) set_flag(Z); else reset_flag(Z);
            if(y & 0x80) set_flag(N); else reset_flag(N);
            break;
        case 0x4c: // JMP Absolute
            pc = (inst->operand2 << 8) | inst->operand1;
            break;
        case 0x6c: // JMP Indirect
            pc = read_memory((inst->operand2 << 8) | inst->operand1);
            // TODO: Implement bug page crossing behavior for JMP Indirect, see https://www.nesdev.org/wiki/Instruction_reference#JMP
            break;
        case 0x20: // JSR Absolute
            push((pc >> 8) & 0xff); // push high byte of PC
            push(pc & 0xff);        // push low byte of PC
            pc = read_memory((inst->operand2 << 8) | inst->operand1);
            break;
        case 0xa9: // LDA Immediate
            LDN(&a, inst->operand1);
            break;
        case 0xa5: // LDA Zero Page
            LDN(&a, read_memory(inst->operand1));
            break;
        case 0xb5: // LDA Zero Page,X
            LDN(&a, read_memory((inst->operand1 + x) & 0xff));
            break;
        case 0xad: // LDA Absolute
            LDN(&a, read_memory(inst->operand2 << 8 | inst->operand1));
            break;
        case 0xbd: // LDA Absolute,X
            if(inst->operand1 + x > 0xff) inst->cycles++; // page crossed
            LDN(&a, read_memory(((inst->operand2 << 8 | inst->operand1) + x) & 0xffff));
            break;
        case 0xb9: // LDA Absolute,Y
            if(inst->operand1 + y > 0xff) inst->cycles++; // page crossed
            LDN(&a, read_memory(((inst->operand2 << 8 | inst->operand1) + y) & 0xffff));
            break;
        case 0xa1: // LDA (Indirect,X)
            addr = get_indirect_x_address(inst->operand1);
            LDN(&a, read_memory(addr & 0xffff));
            break;
        case 0xb1: // LDA (Indirect),Y
            addr = get_indirect_y_address(inst->operand1, &page_crossed);
            if(page_crossed) inst->cycles++; // page crossed
            LDN(&a, read_memory(addr & 0xffff));
            break;
        case 0xa2: // LDX Immediate
            LDN(&x, inst->operand1);
            break;
        case 0xa6: // LDX Zero Page
            LDN(&x, read_memory(inst->operand1));
            break;
        case 0xb6: // LDX Zero Page,Y
            LDN(&x, read_memory((inst->operand1 + y) & 0xff));
            break;
        case 0xae: // LDX Absolute
            LDN(&x, read_memory(inst->operand2 << 8 | inst->operand1));
            break;
        case 0xbe: // LDX Absolute,Y
            if(inst->operand1 + y > 0xff) inst->cycles++; // page crossed
            LDN(&x, read_memory(((inst->operand2 << 8 | inst->operand1) + y) & 0xffff));
            break;
        case 0xa0: // LDY Immediate
            LDN(&y, inst->operand1);
            break;
        case 0xa4: // LDY Zero Page
            LDN(&y, read_memory(inst->operand1));
            break;
        case 0xb4: // LDY Zero Page,X
            LDN(&y, read_memory((inst->operand1 + x) & 0xff));
            break;
        case 0xac: // LDY Absolute
            LDN(&y, read_memory(inst->operand2 << 8 | inst->operand1));
            break;
        case 0xbc: // LDY Absolute,X
            if(inst->operand1 + x > 0xff) inst->cycles++; // page crossed
            LDN(&y, read_memory(((inst->operand2 << 8 | inst->operand1) + x) & 0xffff));
            break;
        case 0x4a: // LSR Accumulator
            a = LSR(a);
            break;
        case 0x46: // LSR Zero Page
            write_memory(inst->operand1, LSR(read_memory(inst->operand1)));
            break;
        case 0x56: // LSR Zero Page,X
            write_memory((inst->operand1 + x) & 0xff, LSR(read_memory((inst->operand1 + x) & 0xff)));
            break;
        case 0x4e: // LSR Absolute
            addr = inst->operand2 << 8 | inst->operand1;
            write_memory(addr, LSR(read_memory(addr)));
            break;
        case 0x5e: // LSR Absolute,X
            addr = ((inst->operand2 << 8 | inst->operand1) + x) & 0xffff;
            write_memory(addr, LSR(read_memory(addr)));
            break;
        case 0x09: // ORA Immediate
            ORA(inst->operand1);
            break;
        case 0x05: // ORA Zero Page
            ORA(read_memory(inst->operand1));
            break;
        case 0x15: // ORA Zero Page,X
            ORA(read_memory((inst->operand1 + x) & 0xff));
            break;
        case 0x0d: // ORA Absolute
            ORA(read_memory(inst->operand2 << 8 | inst->operand1));
            break;
        case 0x1d: // ORA Absolute,X
            if(inst->operand1 + x > 0xff) inst->cycles++; // page crossed
            ORA(read_memory(((inst->operand2 << 8 | inst->operand1) + x) & 0xffff));
            break;
        case 0x19: // ORA Absolute,Y
            if(inst->operand1 + y > 0xff) inst->cycles++; // page crossed
            ORA(read_memory(((inst->operand2 << 8 | inst->operand1) + y) & 0xffff));
            break;
        case 0x01: // ORA (Indirect,X)
            addr = get_indirect_x_address(inst->operand1);
            ORA(read_memory(addr & 0xffff));
            break;
        case 0x11: // ORA (Indirect),Y
            addr = get_indirect_y_address(inst->operand1, &page_crossed);
            if(page_crossed) inst->cycles++; // page crossed
            ORA(read_memory(addr & 0xffff));
            break;
        case 0x48: // PHA Implied
            push(a);
            break;
        case 0x68: // PLA Implied
            a = pull();
            if(a == 0) set_flag(Z); else reset_flag(Z);
            if(a & 0x80) set_flag(N); else reset_flag(N);
            break;
        case 0x08: // PHP Implied
            push(p | B | O); // When pushing P to the stack, the B flag is set to 1
            break;
        case 0x28: // PLP Implied
            p = pull_p();
            // TODO: implement delay interrupt behavior for PLP, see https://www.nesdev.org/wiki/Interrupts#RTI and https://www.nesdev.org/wiki/Instruction_reference#PLP
            break;
        case 0x2a: // ROL Accumulator
            a = ROL(a);
            break;
        case 0x6a: // ROR Accumulator
            a = ROR(a);
            break;
        case 0x26: // ROL Zero Page
            write_memory(inst->operand1, ROL(read_memory(inst->operand1)));
            break;
        case 0x66: // ROR Zero Page
            write_memory(inst->operand1, ROR(read_memory(inst->operand1)));
            break;
        case 0x36: // ROL Zero Page,X
            write_memory((inst->operand1 + x) & 0xff, ROL(read_memory((inst->operand1 + x) & 0xff)));
            break;
        case 0x76: // ROR Zero Page,X
            write_memory((inst->operand1 + x) & 0xff, ROR(read_memory((inst->operand1 + x) & 0xff)));
            break;
        case 0x2e: // ROL Absolute
            addr = inst->operand2 << 8 | inst->operand1;
            write_memory(addr, ROL(read_memory(addr)));
            break;
        case 0x6e: // ROR Absolute
            addr = inst->operand2 << 8 | inst->operand1;
            write_memory(addr, ROR(read_memory(addr)));
            break;
        case 0x3e: // ROL Absolute,X
            addr = ((inst->operand2 << 8 | inst->operand1) + x) & 0xffff;
            write_memory(addr, ROL(read_memory(addr)));
            break;
        case 0x7e: // ROR Absolute,X
            addr = ((inst->operand2 << 8 | inst->operand1) + x) & 0xffff;
            write_memory(addr, ROR(read_memory(addr)));
            break;
        case 0x40: // RTI Implied
            p = pull_p();
            pc = pull() | (pull() << 8);
            break;
            // TODO: implement non-delay interrupt behavior for RTI, see https://www.nesdev.org/wiki/Interrupts#RTI and https://www.nesdev.org/wiki/Instruction_reference#RTI
        case 0x60: // RTS Implied
            pc = (pull() | (pull() << 8)) + 1;
            break;
        case 0xea: // NOP Implied
            break;
        default:
            // For unknown instructions, we can just ignore them or log an error.
            break;
    }
}    

uint16_t CPU::get_indirect_x_address(uint8_t value) {
    int8_t low = read_memory((value + x) & 0xff);
    uint8_t high = read_memory((value + x + 1) & 0xff);
    return read_memory(((high << 8) + low) & 0xffff);
}

uint16_t CPU::get_indirect_y_address(uint8_t value, bool *page_crossed) {
    uint8_t low = read_memory(value);
    uint8_t high = read_memory(value + 1);
    *page_crossed = ((low + y) > 0xff);
    return (read_memory(((high << 8) + low) & 0xffff) + y) & 0xffff;
}

uint8_t CPU::read_memory(uint16_t addr) {
    if(addr >= 0x2000 && addr < 0x4000) {
        // Mirror of PPU registers
        return shared_data->get_ppu_register(addr);
    } else {
        return memory[addr];
    }
}

void CPU::write_memory(uint16_t addr, uint8_t value) {
    if(addr >= 0x2000 && addr < 0x4000) {
        // Mirror of PPU registers
        shared_data->set_ppu_register(addr, value);
        return;
    } else {
        memory[addr] = value;
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
    write_memory(0x100 + s, value);
    s--;
}
uint8_t CPU::pull() {
    s++;
    return read_memory(0x100 + s);
}
uint8_t CPU::pull_p() {
    uint8_t pval = pull() & ~(B | O);
    uint8_t bflags = p & (B | O); 
    return pval | bflags;
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

void CPU::BIT(uint8_t value) {
    uint8_t result = a & value;
    if(result == 0) set_flag(Z); else reset_flag(Z);
    if((result & 0x80) > 0) set_flag(N); else reset_flag(N); 
    if((result & 0x40) > 0) set_flag(V); else reset_flag(V);
}

void CPU::CMP(uint8_t val1, uint8_t val2) {
    if(val1 >= val2) set_flag(C); else reset_flag(C);
    if(val1 == val2) set_flag(Z); else reset_flag(Z);
    if((val1 - val2) & 0x80) set_flag(N); else reset_flag(N);
}

void CPU::EOR(uint8_t value) {
    a = a ^ value;
    if(a == 0) set_flag(Z); else reset_flag(Z);
    if(a & 0x80) set_flag(N); else reset_flag(N);
}

void CPU::LDN(uint8_t *target, uint8_t value) {
    *target = value;
    if(*target == 0) set_flag(Z); else reset_flag(Z);
    if(*target & 0x80) set_flag(N); else reset_flag(N);
}

uint8_t CPU::LSR(uint8_t value) {
    if(value & 0x1) set_flag(C); else reset_flag(C);
    if(value >> 1 == 0) set_flag(Z); else reset_flag(Z);
    reset_flag(N);
    return value >> 1;
}

void CPU::ORA(uint8_t value) {
    a |= value;
    if(a == 0) set_flag(Z); else reset_flag(Z);
    if(a & 0x80) set_flag(N); else reset_flag(N);
}

uint8_t CPU::ROL(uint8_t value) {
    bool old_carry = is_set(C);
    if(value & 0x80) set_flag(C); else reset_flag(C);
    value <<= 1;
    value |= old_carry ? 1 : 0;
    if(value ==0) set_flag(Z); else reset_flag(Z);
    if(value & 0x80) set_flag(N); else reset_flag(N);
    return value;
}

uint8_t CPU::ROR(uint8_t value) {
    bool old_carry = is_set(C);
    if(value & 0x1) set_flag(C); else reset_flag(C);
    value >>= 1;
    value |= old_carry ? 0x80 : 0;
    if(value == 0) set_flag(Z); else reset_flag(Z);
    if(value & 0x80) set_flag(N); else reset_flag(N);
    return value;
}