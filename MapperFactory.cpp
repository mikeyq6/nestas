#include "inc/MapperFactory.h"

#include "inc/mappers/MBC1.h"
#include "inc/mappers/MBC2.h"

Mapper* MapperFactory::get_mapper(MapperType mapperType, SharedData *shared_data) {
    uint8_t rom[10];
    uint32_t rom_size = 10;
    uint8_t ram[20];
    uint32_t ram_size = 20;
    switch(mapperType) {
        case T_MBC1:
            return new MBC1(rom, rom_size, ram, ram_size);
            break;
        case T_MBC2:
            return new MBC2(rom, rom_size, ram, ram_size);
            break;
        default:
            return new MBC1(rom, rom_size, ram, ram_size);
            break;
    }
}