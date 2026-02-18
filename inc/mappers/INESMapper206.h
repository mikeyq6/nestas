#pragma once

#include "Mapper.h"

#include <cstdint>

using std::uint8_t;
using std::uint16_t;

class INESMapper206 : public Mapper {
public:
    INESMapper206(const char *raw_cartridge_data);
    ~INESMapper206();

    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t value) override;
};