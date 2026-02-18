#include "../inc/mappers/NROM.h"

NROM::NROM(const char *raw_cartridge_data) : Mapper(raw_cartridge_data) {
}

NROM::~NROM() {}

uint8_t NROM::read(uint16_t addr) {
    if(addr < 0x2000) {
        return chr_rom[addr];
    } else if(addr >= 0x6000 && addr < 0x8000) {
    } else if(addr >= 0x8000 && addr < 0xc000) {
        return prg_rom[addr - 0x8000];
    } else if(addr >= 0xc000 && addr <= 0xffff) {
        if(prg_rom_size == 0x4000) {
            return prg_rom[addr - 0xc000];
        } else {
            return prg_rom[addr - 0x8000];
        }
    } else {
        // Invalid address
        return 0;
    }
    return 0;
}

void NROM::write(uint16_t addr, uint8_t value) {
    // NROM is read-only, so ignore all writes
    return;
}