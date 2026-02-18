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
    static Mapper* get_mapper(SharedData *shared_data, const char *raw_cartridge_data);

private:
    static uint8_t get_mapper_type(const char *raw_cartridge_data);
};