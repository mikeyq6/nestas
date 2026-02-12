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

private:
	mutex is_running_mutex;
};
