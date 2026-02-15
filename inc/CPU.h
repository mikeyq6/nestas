#pragma once

#include <cstdint>
#include <vector>

#include "Constants.h"
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
    CPU(SharedData *shared_data);
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
    PPU *ppu;

    uint8_t memory[MEMORY_SIZE];

    void set_flag(FLAG flag);
    void reset_flag(FLAG flag);
    bool is_set(FLAG flag);

    void get_next_instruction(uint8_t *inst);
    void decode_instruction(uint8_t cur_inst, Instruction *inst);
    void execute_instruction(Instruction *inst);

    // stack operations
    void push(uint8_t value);
    uint8_t pull();

    // Instruction execution
    void adc(uint8_t value);
};