#pragma once

#include <cstdint>
#include <vector>

#include "Constants.h"

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

class CPU {
public:
    void init();

private:
    CPU();
    ~CPU();

    // registers
    uint8_t a, x, y, p, s;
    uint16_t pc;

    uint8_t memory[MEMORY_SIZE];

    void set_flag(FLAG flag);
    void reset_flag(FLAG flag);
    bool is_set(FLAG flag);
};