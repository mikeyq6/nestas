#include "../inc/mappers/MBC2.h"
    
MBC2::MBC2(const char *raw_cartridge_data) : Mapper(raw_cartridge_data) {
    // this->rom = rom;
    // this->rom_size = rom_size;
    // this->ram = ram;
    // this->ram_size = ram_size;
}
MBC2::~MBC2() {}

uint8_t MBC2::read(uint16_t addr) {
    return 0;
}
void MBC2::write(uint16_t addr, uint8_t value) {
    return;
}