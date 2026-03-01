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
    switch(reg) {
        case OAMDATA:
        case OAMADDR:
        default:
            return registers[reg];
            break;
    }
}
void PPU::set_register(uint8_t reg, uint8_t value) {
    switch(reg) {
        case OAMDATA:
            set_oam_data();
            registers[OAMADDR] = registers[OAMADDR] + 1;
            break;
        case OAMADDR:
        default:
            registers[reg] = value;
            break;
    }
    
}

void PPU::run_cpu_cycle(uint8_t cycles) {
    uint16_t ppu_dots = cycles * 3;

    // TODO: Handle oamaddr bug when value > 8, see https://www.nesdev.org/wiki/PPU_registers#Values_during_rendering

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
            // reset OAMADDR
            set_register(OAMADDR, 0);
        } else if(scanline_counter > NUM_SCANLINES) {
            // End of vblank, start of new frame
            clear_vblank();
            scanline_counter = 0;
        }
    } else {
        dot_counter += ppu_dots;
    }
}

void PPU::set_oam_data() {
    uint8_t data = get_register(OAMDATA);
    uint8_t oamaddr = get_register(OAMADDR);
    uint8_t index = oamaddr / 4;
    switch(oamaddr % 4) {
        case 0:
            oam_data[index].y = data;
            break;
        case 1:
            oam_data[index].tile_index = data;
            break;
        case 2:
            oam_data[index].attributes = data;
            break;
        case 3:
            oam_data[index].x = data;
            break;
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