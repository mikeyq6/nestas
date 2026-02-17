#include "../inc/mappers/MBC2.h"
    
MBC2::MBC2(uint8_t *rom, uint32_t rom_size, uint8_t *ram, uint32_t ram_size) {
    this->rom = rom;
    this->rom_size = rom_size;
    this->ram = ram;
    this->ram_size = ram_size;
}
MBC2::~MBC2() {}

uint8_t MBC2::read(uint16_t addr) {
    return 0;
}
void MBC2::write(uint16_t addr, uint8_t value) {
    return;
}