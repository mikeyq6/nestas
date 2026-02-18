#pragma once

#include "mappers/Mapper.h"
#include "SharedData.h"

typedef enum _mapperType {
    T_MBC1 = 0x01,
    T_MBC2 = 0x02
} MapperType;

class MapperFactory
{
public:
    static Mapper* get_mapper(SharedData *shared_data, const char *cartridge_data);
};