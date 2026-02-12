#include "inc/SharedData.h"

SharedData::SharedData() {
    is_running = true;
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