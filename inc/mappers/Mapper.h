#pragma once

#include <cstdint>

using std::uint8_t;
using std::uint32_t;

typedef enum romFormat {
    NES,
    NES2
} RomFormat;

class Mapper {
public:
    Mapper(const char *raw_cartridge_data);
    virtual ~Mapper() = default;
    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t value) = 0;

protected:
    RomFormat rom_format;
    // uint8_t *rom;
    // uint32_t rom_size;
    // uint8_t *ram;
    // uint32_t ram_size;
};