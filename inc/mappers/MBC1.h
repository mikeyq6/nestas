#pragma once

#include "Mapper.h"

class MBC1 : public Mapper {
public:
    MBC1(const char *raw_cartridge_data);
    ~MBC1();

    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t value) override;
};