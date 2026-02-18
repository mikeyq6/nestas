#include "../inc/mappers/MMC2.h"
    
MMC2::MMC2(const char *raw_cartridge_data) : Mapper(raw_cartridge_data) {
    // this->rom = rom;
    // this->rom_size = rom_size;
    // this->ram = ram;
    // this->ram_size = ram_size;
}
MMC2::~MMC2() {}

uint8_t MMC2::read(uint16_t addr) {
    return 0;
}
void MMC2::write(uint16_t addr, uint8_t value) {
    return;
}