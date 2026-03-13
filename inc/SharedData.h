#pragma once

#include <mutex>
#include <random>
#include <vector>

#include "Constants.h"

using std::mutex;
using std::lock_guard;
using u32 = uint_least32_t; 
using engine = std::mt19937;

class SharedData {
public:
    SharedData();
    ~SharedData();

    bool is_running, show_tile_map;

    bool get_is_running();
    void set_is_running(bool value);

    bool get_show_tile_map();
    void set_show_tile_map(bool value);

    uint16_t get_random(uint16_t max);

    uint8_t get_apu_io_register(uint16_t addr);
    void set_apu_io_register(uint16_t addr, uint8_t value);

    void copy_pixels_from(uint8_t *buffer);
    void copy_pixels_to(uint8_t *buffer);

private:
	mutex is_running_mutex;
    mutex apu_io_register_mutex;
    mutex pixel_buffer_mutex;

    uint8_t pixel_buffer[NUM_PIXELS];
    uint8_t apu_io_registers[0x20];
};
