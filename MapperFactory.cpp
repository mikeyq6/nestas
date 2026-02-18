#include "inc/MapperFactory.h"

#include "inc/mappers/NROM.h"
#include "inc/mappers/MBC1.h"
#include "inc/mappers/MBC2.h"

Mapper* MapperFactory::get_mapper(SharedData *shared_data, const char *cartridge_data) {
    uint8_t mapperType = cartridge_data[0x7] & 0xf;
    
    switch(mapperType) {
        case 0x00:
            return new NROM(cartridge_data);
            break;
        case 0x01:
            return new MBC1(cartridge_data);
            break;
        case 0x02:
            return new MBC2(cartridge_data);
            break;
        default:
            return new MBC1(cartridge_data);
            break;
    }
}