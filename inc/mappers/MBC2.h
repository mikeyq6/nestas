#pragma once

#include "Mapper.h"

class MBC2 : public Mapper {
public:
    MBC2(uint8_t *rom, uint32_t rom_size, uint8_t *ram, uint32_t ram_size);
    ~MBC2();

    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t value) override;
};