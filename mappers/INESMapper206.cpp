#include "../inc/mappers/INESMapper206.h"

INESMapper206::INESMapper206(const char *raw_cartridge_data) : Mapper(raw_cartridge_data) {
}

INESMapper206::~INESMapper206() {}

uint8_t INESMapper206::read(uint16_t addr) {
    return 0;
}

void INESMapper206::write(uint16_t addr, uint8_t value) {
    return;
}