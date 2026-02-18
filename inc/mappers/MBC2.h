#pragma once

#include "Mapper.h"

class MBC2 : public Mapper {
public:
    MBC2(const char *raw_cartridge_data);
    ~MBC2();

    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t value) override;
};