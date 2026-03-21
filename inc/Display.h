#pragma once

#include "Constants.h"
#include "SharedData.h"

class Display {
public:
    Display(SharedData *shared_data);
    virtual ~Display() = default;

    virtual void init() = 0;
    virtual void draw() = 0;
    virtual void set_tile_map_pixels() = 0;
    virtual void set_nametable_map_pixels() = 0;

protected:
    SharedData *shared_data;
    uint32_t pixels[NUM_PIXELS];
    uint8_t tile_map_data[TILE_MAP_PIXELS];
    uint32_t tile_map_pixels[TILE_MAP_PIXELS];
    uint8_t nametable_data[NUM_NAMETABLE_TILES];
    uint32_t nametable_map_pixels[NAMETABLE_MAP_PIXELS];
};