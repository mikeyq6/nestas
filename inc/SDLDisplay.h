#pragma once

#include <thread>
#include <vector>

#include <SDL2/SDL.h>

#include "Display.h"

using std::fill;

class SDLDisplay : public Display
{
public:
    SDLDisplay(SharedData *shared_data);
    ~SDLDisplay();

    void draw() override;
    void init() override;
    void set_tile_map_pixels() override;
    void set_nametable_map_pixels() override;

private:
    bool quit;
    int xpos, ypos, flags, zoom, tile_map_palette_cycle;
	SDL_Window *window, *tile_map_window, *nametable_map_window;
	SDL_Renderer *renderer, *tile_map_renderer, *nametable_map_renderer;
	SDL_Texture *texture, *tile_map_texture, *nametable_map_texture;

    constexpr static uint32_t TILE_MAP_PALETTE_COLOURS[0x40] = {
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00626262, 0x001553d8, 0x003f3cfb, 0x00712bef, 0x009a1eb7, 0x00ae1962, 0x00a72812, 0x0088460e,
        0x0057650f, 0x00287e12, 0x00118814, 0x00108236, 0x000e6e8f, 0x009a1eb7, 0x0057650f, 0x001553d8,
        0x00ababab, 0x005ba7fc, 0x00838bfc, 0x00b372fc, 0x00dd67fc, 0x00f667c5, 0x00f67570, 0x00dd9030,
        0x00b3ad23, 0x0083c625, 0x005cd335, 0x0046d273, 0x0046c1c7, 0x004e4e4e, 0x00712bef, 0x0046d273,
        0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff,
        0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff
    };
    constexpr static uint32_t PALETTE_COLOURS[0x40] = { // Array of colours to map from Hue/Lumnance values to SDL colours
        0x00626262, 0x00042393, 0x001b17a9, 0x0042129b, 0x00600a6a, 0x006d0326, 0x00640705, 0x00481e04, 
        0x00233704, 0x00054806, 0x00064e07, 0x00054819, 0x0004365d, 0x00000000, 0x00000000, 0x00000000,
        0x00ababab, 0x001553d8, 0x003f3cfb, 0x00712bef, 0x009a1eb7, 0x00ae1962, 0x00a72812, 0x0088460e,
        0x0057650f, 0x00287e12, 0x00118814, 0x00108236, 0x000e6e8f, 0x00000000, 0x00000000, 0x00000000,
        0x00ffffff, 0x005ba7fc, 0x00838bfc, 0x00b372fc, 0x00dd67fc, 0x00f667c5, 0x00f67570, 0x00dd9030,
        0x00b3ad23, 0x0083c625, 0x005cd335, 0x0046d273, 0x0046c1c7, 0x004e4e4e, 0x00000000, 0x00000000,
        0x00ffffff, 0x00bfe1fe, 0x00ced5fe, 0x00e0cbfe, 0x00f0c6fe, 0x00fbc5ee, 0x00fccbcf, 0x00f4d4b1,
        0x00e6dea0, 0x00d4e89e, 0x00c3eeab, 0x00b9eec5, 0x00b8eae5, 0x00b8b8b8, 0x00000000, 0x00000000
    };

    void process_key_event(SDL_Event* event);
    void get_tile_data(uint8_t *tile, uint8_t *tile_map_data, uint8_t tile_number);
    
};