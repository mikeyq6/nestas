#include "inc/PPU.h"

PPU::PPU(SharedData *shared_data, Mapper *mapper) {
    this->shared_data = shared_data;
    this->mapper = mapper;
}
PPU::~PPU() {

}

void PPU::init() {
    for(int i=0; i<8; i++) {
        set_register(i, 0);
    }
    dot_counter = 0;
    current_scanline = 0;
    sprite_counter = 0;
    for(int i=0; i<OAM_SIZE; i++) {
        oam_data[i] = {0, 0, 0, 0};
    }
    w = 0;
    x_scroll = y_scroll = ppu_addr = 0;
    vram_increment = 0;
    base_nametable_addr = 0x2000; // Default to nametable 0
    sprite_pattern_base_addr = 0; // Default to 8x8 sprites with pattern table 0
    background_base_addr = 0; // Default to pattern table 0 for background
    sprite_size = 0;
    std::fill(palette_indexes, palette_indexes + PALETTE_SIZE, 0);
}

uint8_t PPU::get_register(uint8_t reg) {
    switch(reg) {
        case PPUSTATUS: 
            return registers[PPUSTATUS] & 0xe0; // Only bits 7-5 are readable
            break;
        case PPUMASK:
        case PPUCTRL:
        case PPUSCROLL:
        case PPUADDR:
            return 0; 
            break;
        case OAMDATA:
        case OAMADDR:
        case PPUDATA:
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
        case PPUSTATUS: // Writing to this register has no effect
            break;
        case PPUSCROLL:
            if(w == 0) {
                x_scroll = value;
            } else {
                y_scroll = value;
            }
            w = (w + 1) % 2;
            break;
        case PPUADDR:
            if(w == 0) {
                ppu_addr |= value;
            } else {
                ppu_addr = (value & 0x3f) << 8 | x_scroll; // Only bits 13-8 are writable, lower 8 bits come from first write
            }
            w = (w + 1) % 2;
            break;
        case PPUDATA:
            registers[reg] = value;
            registers[PPUADDR] = (registers[PPUADDR] + vram_increment) & 0xffff; // Increment PPUADDR after write
            break;
        case PPUCTRL:
            vram_increment = (value & 0x4) ? 32 : 1; // Set VRAM address increment based on bit 2 of PPUCTRL
            switch(value & 0x3) { // Set base nametable address based on bits 1-0 of PPUCTRL
                case 0:
                    base_nametable_addr = 0x2000;
                    break;
                case 1:
                    base_nametable_addr = 0x2400;
                    break;
                case 2:
                    base_nametable_addr = 0x2800;
                    break;
                case 3:
                    base_nametable_addr = 0x2c00;
                    break;
            }
            sprite_pattern_base_addr = (value & 0x8) ? 0x1000 : 0;
            background_base_addr = (value & 0x10) ? 0x1000 : 0;
            sprite_size = (value & 0x20) ? 16 : 8;
            vblank_enable = (value & 0x80) != 0 ? 1 : 0;
            break;
        case OAMADDR:
        case PPUMASK:
        default:
            registers[reg] = value;
            break;
    }
    
}

void PPU::run_cpu_cycle(uint8_t cycles) {
    uint16_t ppu_dots = cycles * 3;
    ext_palette = read_address(EXT_ADDRESS);

    // TODO: Handle oamaddr bug when value > 8, see https://www.nesdev.org/wiki/PPU_registers#Values_during_rendering

    if(is_rendering_enabled()) {
        set_pixels_for(ppu_dots);
    }
    if(ppu_dots + dot_counter >= NUM_DOTS) {
        // End of scanline
        dot_counter = (dot_counter + ppu_dots) % NUM_DOTS;
        current_scanline++;
        if(current_scanline == VISIBLE_SCANLINES + 1 && dot_counter > 0) { 
            // Start of vblank
            set_vblank();
            // reset OAMADDR
            set_register(OAMADDR, 0);
        } else if(current_scanline > NUM_SCANLINES) {
            // End of vblank, start of new frame
            clear_vblank();
            current_scanline = 0;
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

void PPU::set_oam_buffer() {
    sprite_counter = 0;

    for(int i=0; i<OAM_SIZE; i++) {
        oam sprite = oam_data[i];

        if(sprite_is_in_scanline(sprite)) {
            if(sprite_counter < MAX_SPRITES) {
                // Add sprite to buffer
                oam_buffer[sprite_counter++] = sprite;
            } else {
                // TODO: Set sprite overflow flag and implement bug
            }
        }
    }
}

bool PPU::sprite_is_in_scanline(oam sprite) {
    // case for 8x8 sprites, TODO: 8x16
    return (sprite.y > current_scanline - 8 && sprite.y <= current_scanline);
}

void PPU::oam_dma(uint8_t *oam_buffer) {
    for(int i=0; i<OAM_BUFFER_SIZE; i++) {
        uint8_t data = oam_buffer[i];
        uint8_t index = i / 4;
        switch(i % 4) {
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

uint8_t PPU::read_address(uint16_t addr) {
    if(addr < 0x3f00) {
        return mapper->read(addr);
    } else if(addr >= 0x3f00 && addr < 0x3f20) {
        return palette_indexes[addr - 0x3f00];
    } else {
        return 0; // Open bus behavior for addresses that are not handled
    }
}