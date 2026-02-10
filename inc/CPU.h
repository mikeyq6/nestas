#pragma once

#include <cstdint>
#include <vector>

#include "Constants.h"
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

    uint8_t memory[MEMORY_SIZE];

    void set_flag(FLAG flag);
    void reset_flag(FLAG flag);
    bool is_set(FLAG flag);
};