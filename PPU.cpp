#include "inc/PPU.h"

PPU::PPU(SharedData *shared_data) {
    this->shared_data = shared_data;
}
PPU::~PPU() {

}

void PPU::init() {
    for(int i=0; i<8; i++) {
        set_register(i, 0);
    }
    dot_counter = 0;
    scanline_counter = 0;
    for(int i=0; i<OAM_SIZE; i++) {
        oam_data[i] = {0, 0, 0, 0};
    }
}

uint8_t PPU::get_register(uint8_t reg) {
    return registers[reg];
}
void PPU::set_register(uint8_t reg, uint8_t value) {
    registers[reg] = value;
}

void PPU::run_cpu_cycle(uint8_t cycles) {
    uint16_t ppu_dots = cycles * 3;

    if(is_rendering_enabled()) {
        set_pixels_for(ppu_dots);
    }
    if(ppu_dots + dot_counter >= NUM_DOTS) {
        // End of scanline
        dot_counter = (dot_counter + ppu_dots) % NUM_DOTS;
        scanline_counter++;
        if(scanline_counter == VISIBLE_SCANLINES + 1 && dot_counter > 0) { 
            // Start of vblank
            set_vblank();
        } else if(scanline_counter > NUM_SCANLINES) {
            // End of vblank, start of new frame
            clear_vblank();
            scanline_counter = 0;
        }
    } else {
        dot_counter += ppu_dots;
    }
}

void PPU::set_pixels_for(uint16_t num_dots) {
    // TODO
}

bool PPU::is_rendering_enabled() {
    uint8_t ppumask = get_register(PPUMASK);
    return (ppumask & 0x18) != 0; // At least one of background and sprites are enabled
}

void PPU::set_vblank() {
    uint8_t ppustatus = get_register(PPUSTATUS);
    set_register(PPUSTATUS, ppustatus | 0x80);
}
void PPU::clear_vblank() {
    uint8_t ppustatus = get_register(PPUSTATUS);
    set_register(PPUSTATUS, ppustatus & 0x7f);
}