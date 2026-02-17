#pragma once

#include <mutex>
#include <random>

using std::mutex;
using std::lock_guard;
using u32 = uint_least32_t; 
using engine = std::mt19937;

class SharedData {
public:
    SharedData();
    ~SharedData();

    bool is_running;

    bool get_is_running();
    void set_is_running(bool value);

    uint16_t get_random(uint16_t max);

    uint8_t get_ppu_register(uint16_t addr);
    void set_ppu_register(uint16_t addr, uint8_t value);

    uint8_t get_apu_io_register(uint16_t addr);
    void set_apu_io_register(uint16_t addr, uint8_t value);

private:
	mutex is_running_mutex;
    mutex ppu_register_mutex;
    mutex apu_io_register_mutex;

    uint8_t ppu_registers[8];
    uint8_t apu_io_registers[0x20];
};
