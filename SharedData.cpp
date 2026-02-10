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