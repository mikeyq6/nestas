#pragma once

#include <cstdint>
#include <vector>

#include "Constants.h"
#include "SharedData.h"

using std::uint8_t;

typedef struct oam {
    uint8_t y;
    uint8_t tile_index;
    uint8_t attributes;
    uint8_t x;
} Oam;

class PPU {
public:
    PPU(SharedData *shared_data);
    ~PPU();

    void init();
    void run_cpu_cycle(uint8_t cycles);

    uint8_t get_register(uint8_t reg);
    void set_register(uint8_t reg, uint8_t value);

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

private:

    uint8_t registers[8];

    uint16_t dot_counter;
    uint16_t scanline_counter;

    Oam oam_data[OAM_SIZE];

    SharedData *shared_data;

    void set_pixels_for(uint16_t num_dots);
    bool is_rendering_enabled();
    void set_vblank();
    void clear_vblank();

};
