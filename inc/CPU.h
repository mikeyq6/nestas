#pragma once

#include <cstdint>
#include <vector>

#include "Constants.h"
#include "mappers/Mapper.h"
#include "PPU.h"
#include "SharedData.h"

using std::uint8_t;
using std::uint16_t;

enum FLAG {
    C = 0x01,
    Z = 0x02,
    I = 0x04,
    D = 0x08,
    B = 0x10,
    O = 0x20, // Unused, always set to 1 when pushed to stack
    V = 0x40,
    N = 0x80
};

typedef struct _instruction {
    uint8_t opcode;
    uint8_t operand1;
    uint8_t operand2;
    uint8_t cycles;
} Instruction;

class CPU {
public:
    CPU(SharedData *shared_data, PPU *ppu, Mapper *mapper);
    ~CPU();

    void init();
    void reset();
    void run();
    void stop();

private:

    // registers
    uint8_t a, x, y, p, s;
    uint16_t pc;

    SharedData *shared_data;
    Mapper *mapper;
    PPU *ppu;

    uint8_t memory[MEMORY_SIZE];
    uint8_t oam_buffer[OAM_BUFFER_SIZE]; // Buffer for OAM DMA

    void set_flag(FLAG flag);
    void reset_flag(FLAG flag);
    bool is_set(FLAG flag);

    void get_next_instruction(uint8_t *inst);
    void decode_instruction(uint8_t cur_inst, Instruction *inst);
    void execute_instruction(Instruction *inst);
    uint16_t get_indirect_x_address(uint8_t value);
    uint16_t get_indirect_y_address(uint8_t value, bool *page_crossed);

    uint8_t read_memory(uint16_t addr);
    void write_memory(uint16_t addr, uint8_t value);

    // stack operations
    void push(uint8_t value);
    uint8_t pull();
    uint8_t pull_p();

    // Instruction execution
    void ADC(uint8_t value);
    void AND(uint8_t value);
    uint8_t ASL(uint8_t value);
    void BIT(uint8_t value);
    void CMP(uint8_t val1, uint8_t val2);
    void EOR(uint8_t value);
    void LDN(uint8_t *target, uint8_t value);
    uint8_t LSR(uint8_t value);
    void ORA(uint8_t value);
    uint8_t ROL(uint8_t value);
    uint8_t ROR(uint8_t value);

    void dma(uint8_t value);
};