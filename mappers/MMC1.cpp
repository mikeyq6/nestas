#include "../inc/mappers/MMC1.h"
    
MMC1::MMC1(const char *raw_cartridge_data) : Mapper(raw_cartridge_data) {
}
MMC1::~MMC1() {}

uint8_t MMC1::read(uint16_t addr) {
    if(addr < 0x1000) {
        // TODO: Implement switchable CHR bank
        return chr_rom[addr];
    } else if(addr >= 0x1000 && addr < 0x2000) {
        // TODO: Implement switchable CHR bank
        return chr_rom[addr];
    } else if(addr >= 0x6000 && addr < 0x8000) {
        // TODO: Implement RAM banking
    } else if(addr >= 0x8000 && addr < 0xc000) {
        // TODO: Implement ROM banking (low)
    } else if(addr >= 0xc000 && addr <= 0xffff) {
        // TODO: Implement ROM banking (high)
    } else {
        // Invalid address
        return 0;
    }
    return 0;
}

void MMC1::write(uint16_t addr, uint8_t value) {
    if(addr >= 0x6000 && addr < 0x8000) {
        // TODO: Implement RAM banking
    } else if(addr >= 0x8000 && addr <= 0xffff) {
        // TODO: Clear shift register
    }
    return;
}