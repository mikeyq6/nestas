#pragma once

#include <cstdint>

#include "Constants.h"
#include "SharedData.h"

class PPU {
public:
    PPU(SharedData *shared_data);
    ~PPU();

    void init();

private:
    enum PPURegister {
        PPUCTRL = 0x0,
        PPUMASK = 0x1,
        PPUSTATUS = 0x2,
        OAMADDR = 0x3,
        OAMDATA = 0x4,
        PPUSCROLL = 0x5,
        PPUADDR = 0x6,
        PPUDATA = 0x7
    };

    SharedData *shared_data;
};
