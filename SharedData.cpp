#include "inc/SharedData.h"

SharedData::SharedData() {
    is_running = true;
    std::fill(pixels, pixels + NUM_PIXELS, 0);
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

uint16_t SharedData::get_random(uint16_t max) {
    // Init random number generator
    std::random_device os_seed;
    const u32 seed = os_seed();
    engine generator(seed);
    std::uniform_int_distribution<u32> distribute(0, max);
    return distribute(generator);
}

uint8_t SharedData::get_ppu_register(uint16_t addr) {
    const lock_guard<mutex> lock{ppu_register_mutex};

    return ppu_registers[addr & 0x7];
}
void SharedData::set_ppu_register(uint16_t addr, uint8_t value) {
    const lock_guard<mutex> lock{ppu_register_mutex};

    ppu_registers[addr & 0x7] = value;
}

uint8_t SharedData::get_apu_io_register(uint16_t addr) {
    const lock_guard<mutex> lock{apu_io_register_mutex};

    return apu_io_registers[addr & 0x20];
}
void SharedData::set_apu_io_register(uint16_t addr, uint8_t value) {
    const lock_guard<mutex> lock{apu_io_register_mutex};

    apu_io_registers[addr & 0x7] = value;
}