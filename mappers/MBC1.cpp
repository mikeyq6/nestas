#include "../inc/mappers/MBC1.h"
    
MBC1::MBC1(const char *raw_cartridge_data) : Mapper(raw_cartridge_data) {
}
MBC1::~MBC1() {}

uint8_t MBC1::read(uint16_t addr) {
    return 0;
}
void MBC1::write(uint16_t addr, uint8_t value) {
    return;
}