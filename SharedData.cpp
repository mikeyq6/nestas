#include "inc/SharedData.h"

SharedData::SharedData() {
    is_running = true;
    show_tile_map = false;
    std::fill(pixel_buffer, pixel_buffer + NUM_PIXELS, 0);
    std::fill(tile_map_pixel_buffer, tile_map_pixel_buffer + TILE_MAP_DATA_SIZE, 0);
}
SharedData::~SharedData() {

}

bool SharedData::get_is_running() {
    const lock_guard<mutex> lock{is_running_mutex};

    return is_running;
}
void SharedData::set_is_running(bool value) {
    const lock_guard<mutex> lock{is_running_mutex};

    is_running = value;
}

bool SharedData::get_show_tile_map() {
    const lock_guard<mutex> lock(tile_map_pixels_mutex);

    return show_tile_map;
}
void SharedData::set_show_tile_map(bool value) {
    const lock_guard<mutex> lock(tile_map_pixels_mutex);

    show_tile_map = value;
}

bool SharedData::get_show_nametables() {
    const lock_guard<mutex> lock(nametables_mutex);

    return show_nametables;
}
void SharedData::set_show_nametables(bool value) {
    const lock_guard<mutex> lock(nametables_mutex);

    show_nametables = value;
}

uint16_t SharedData::get_random(uint16_t max) {
    // Init random number generator
    std::random_device os_seed;
    const u32 seed = os_seed();
    engine generator(seed);
    std::uniform_int_distribution<u32> distribute(0, max);
    return distribute(generator);
}

uint8_t SharedData::get_apu_io_register(uint16_t addr) {
    const lock_guard<mutex> lock{apu_io_register_mutex};

    return apu_io_registers[addr & 0x20];
}
void SharedData::set_apu_io_register(uint16_t addr, uint8_t value) {
    const lock_guard<mutex> lock{apu_io_register_mutex};

    apu_io_registers[addr & 0x7] = value;
}

void SharedData::copy_pixels_from(uint8_t *buffer) {
    const lock_guard<mutex> lock{pixel_buffer_mutex};

    std::copy(buffer, buffer + NUM_PIXELS, pixel_buffer);
}
void SharedData::copy_pixels_to(uint8_t *buffer) {
    const lock_guard<mutex> lock{pixel_buffer_mutex};

    std::copy(pixel_buffer, pixel_buffer + NUM_PIXELS, buffer);
}

void SharedData::copy_tile_map_pixels_from(uint8_t *buffer) {
    const lock_guard<mutex> lock{tile_map_pixels_mutex};

    std::copy(buffer, buffer + TILE_MAP_DATA_SIZE, tile_map_pixel_buffer);
}
void SharedData::copy_tile_map_pixels_to(uint8_t *buffer) {
    const lock_guard<mutex> lock{tile_map_pixels_mutex};

    std::copy(tile_map_pixel_buffer, tile_map_pixel_buffer + TILE_MAP_DATA_SIZE, buffer);
}

void SharedData::copy_nametable_map_pixels_from(uint8_t *buffer) {
    const lock_guard<mutex> lock{nametables_mutex};

    std::copy(buffer, buffer + NAMETABLE_MAP_PIXELS, nametables_map_pixel_buffer);

}
void SharedData::copy_nametable_map_pixels_to(uint8_t *buffer) {
    const lock_guard<mutex> lock{nametables_mutex};

    std::copy(nametables_map_pixel_buffer, nametables_map_pixel_buffer + NAMETABLE_MAP_PIXELS, buffer);
}

bool SharedData::get_nmi_pending() {
    return nmi_pending;
}
void SharedData::set_nmi_pending(bool value) {
    nmi_pending = value;
}

bool SharedData::get_irq_pending() {
    return irq_pending;
}
void SharedData::set_irq_line(uint8_t line) {
    irq_pending = true;
}