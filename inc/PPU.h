#pragma once

#include <cstdint>
#include <vector>

#include "Constants.h"
#include "SharedData.h"
#include "mappers/Mapper.h"

using std::uint8_t;

typedef struct oam {
    uint8_t y;
    uint8_t tile_index;
    uint8_t attributes;
    uint8_t x;
} Oam;

class PPU {
public:
    PPU(SharedData *shared_data, Mapper *mapper);
    ~PPU();

    void init();
    void run_cpu_cycle(uint8_t cycles);

    uint8_t get_register(uint8_t reg);
    void set_register(uint8_t reg, uint8_t value);
    void oam_dma(uint8_t *oam_buffer);

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
    uint8_t vram[VRAM_SIZE];
    uint8_t w, sprite_size, vblank_enable;
    uint16_t x_scroll, y_scroll, ppu_addr, base_nametable_addr, sprite_pattern_base_addr, 
        background_base_addr;
    uint8_t vram_increment;

    uint16_t dot_counter;
    uint16_t current_scanline;

    uint8_t ext_palette;
    uint8_t palette_indexes[PALETTE_SIZE];

    Mapper *mapper;

    Oam oam_data[OAM_SIZE];
    uint8_t sprite_counter;
    Oam oam_buffer[MAX_SPRITES]; // Buffer for sprites on current scanline
    uint8_t background_for_scanline[TILES_IN_SCANLINE]; // Buffer for background tiles on current scanline
    uint8_t tile_map_pixels_buffer[TILE_MAP_DATA_SIZE];
    uint8_t nametable_pixels_buffer[NUM_NAMETABLE_TILES];

    void set_oam_data();
    void set_oam_buffer();
    void set_background_tiles_for_scanline();
    bool sprite_is_in_scanline(oam sprite);

    SharedData *shared_data;

    void set_pixels_for(uint16_t num_dots);
    bool is_rendering_enabled();
    void set_vblank();
    void clear_vblank();

    uint8_t read_address(uint16_t addr);

    void set_tile_map_data();
    void set_nametable_data();
};
