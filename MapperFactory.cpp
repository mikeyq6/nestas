#include "inc/MapperFactory.h"

#include "inc/mappers/NROM.h"
#include "inc/mappers/MBC1.h"
#include "inc/mappers/MBC2.h"

Mapper* MapperFactory::get_mapper(SharedData *shared_data, const char *raw_cartridge_data) {
    uint8_t mapper_type = get_mapper_type(raw_cartridge_data);
    
    switch(mapper_type) {
        case 0x00:
            return new NROM(raw_cartridge_data);
            break;
        case 0x01:
            return new MBC1(raw_cartridge_data);
            break;
        case 0x02:
            return new MBC2(raw_cartridge_data);
            break;
        default:
            return new MBC1(raw_cartridge_data);
            break;
    }
}

uint8_t MapperFactory::get_mapper_type(const char *raw_cartridge_data) {
    if (raw_cartridge_data[0] == 'N' && raw_cartridge_data[1] == 'E' && raw_cartridge_data[2] == 'S' && 
        raw_cartridge_data[3] == 0x1a && (raw_cartridge_data[7] & 0x0c) == 0x08) {
        return ((raw_cartridge_data[8] & 0xf) << 8) | ((raw_cartridge_data[6] & 0xf0) >> 4) | (raw_cartridge_data[7] & 0xf0);
    } else {
        return ((raw_cartridge_data[6] & 0xf0) >> 4) | (raw_cartridge_data[7] & 0xf0);
    }
}