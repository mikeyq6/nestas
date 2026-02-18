#include "../inc/mappers/UxROM.h"

// https://www.nesdev.org/wiki/UxROM

UxROM::UxROM(const char *raw_cartridge_data) : Mapper(raw_cartridge_data) {
}

UxROM::~UxROM() {}

uint8_t UxROM::read(uint16_t addr) {
    return 0;
}

void UxROM::write(uint16_t addr, uint8_t value) {
    return;
}