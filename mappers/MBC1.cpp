#include "../inc/mappers/MBC1.h"
    
MBC1::MBC1(const char *raw_cartridge_data) : Mapper(raw_cartridge_data) {
}
MBC1::~MBC1() {}

uint8_t MBC1::read(uint16_t addr) {
    if(addr < 0x1000) {
        // TODO: Implement switchable CHR bank
    } else if(addr >= 0x1000 && addr < 0x2000) {
        // TODO: Implement switchable CHR bank
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

void MBC1::write(uint16_t addr, uint8_t value) {
    if(addr >= 0x6000 && addr < 0x8000) {
        // TODO: Implement RAM banking
    } else if(addr >= 0x8000 && addr <= 0xffff) {
        // TODO: Clear shift register
    }
    return;
}