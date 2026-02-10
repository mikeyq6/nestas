#pragma once

#include <cstdint>

class PPU {
public:
    PPU();
    ~PPU();

    void init();

private:
    enum PPURegister {
        PPUCTRL = 0x2000,
        PPUMASK = 0x2001,
        PPUSTATUS = 0x2002,
        OAMADDR = 0x2003,
        OAMDATA = 0x2004,
        PPUSCROLL = 0x2005,
        PPUADDR = 0x2006,
        PPUDATA = 0x2007
    };

    uint8_t ppuctrl, ppumask, ppustatus, oamaddr, oamdata, ppuscroll, ppuaddr, ppudata;
};
