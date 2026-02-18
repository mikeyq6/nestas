#pragma once

#include "Mapper.h"

class MMC2 : public Mapper {
public:
    MMC2(const char *raw_cartridge_data);
    ~MMC2();

    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t value) override;
};