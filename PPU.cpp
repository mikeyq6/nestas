#include "inc/PPU.h"

PPU::PPU(SharedData *shared_data) {
    this->shared_data = shared_data;
}
PPU::~PPU() {

}

void PPU::init() {
    for(int i=0; i<0x8; i++) {
        shared_data->set_ppu_register(i, 0);
    }
    dot_counter = 0;
    scanline_counter = 0;
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
    uint8_t ppumask = shared_data->get_ppu_register(PPUMASK);
    return (ppumask & 0x18) != 0; // At least one of background and sprites are enabled
}

void PPU::set_vblank() {
    uint8_t ppustatus = shared_data->get_ppu_register(PPUSTATUS);
    shared_data->set_ppu_register(PPUSTATUS, ppustatus | 0x80);
}
void PPU::clear_vblank() {
    uint8_t ppustatus = shared_data->get_ppu_register(PPUSTATUS);
    shared_data->set_ppu_register(PPUSTATUS, ppustatus & 0x7f);
}