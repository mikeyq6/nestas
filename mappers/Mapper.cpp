#include "../inc/mappers/Mapper.h"

Mapper::Mapper(const char *raw_cartridge_data) {
    if (raw_cartridge_data[0] == 'N' && raw_cartridge_data[1] == 'E' && raw_cartridge_data[2] == 'S' && 
        raw_cartridge_data[3] == 0x1a && (raw_cartridge_data[7] & 0x0c) == 0x08) {
        rom_format = NES2;
    } else {
        rom_format = NES;
    }
}