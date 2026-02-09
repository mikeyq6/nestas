#pragma once

#include <cstdint>
#include <vector>

using std::uint8_t;
using std::uint16_t;

constexpr auto MEMORY_SIZE = 0xffff;

class CPU {
private:
    CPU();
    ~CPU();

    // registers
    uint8_t a, x, y, p;
    uint16_t pc, sp;

    uint8_t memory[MEMORY_SIZE];
};