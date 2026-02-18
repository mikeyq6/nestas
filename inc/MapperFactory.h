#pragma once

#include "mappers/Mapper.h"
#include "SharedData.h"

class MapperFactory
{
public:
    static Mapper* get_mapper(SharedData *shared_data, const char *raw_cartridge_data);

private:
    static uint8_t get_mapper_type(const char *raw_cartridge_data);
};