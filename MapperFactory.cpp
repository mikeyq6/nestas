#include "inc/MapperFactory.h"

#include "inc/mappers/MBC1.h"
#include "inc/mappers/MBC2.h"

Mapper* MapperFactory::get_mapper(SharedData *shared_data, const char *cartridge_data) {
    uint8_t rom[10];
    uint32_t rom_size = 10;
    uint8_t ram[20];
    uint32_t ram_size = 20;
    uint8_t mapperType = cartridge_data[0x7] & 0xf;
    switch(mapperType) {
        case 0x01:
            return new MBC1(rom, rom_size, ram, ram_size);
            break;
        case 0x02:
            return new MBC2(rom, rom_size, ram, ram_size);
            break;
        default:
            return new MBC1(rom, rom_size, ram, ram_size);
            break;
    }
}