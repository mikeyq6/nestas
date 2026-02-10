#pragma once

#include <mutex>

using std::mutex;
using std::lock_guard;

class SharedData {
public:
    SharedData();
    ~SharedData();

    bool is_running;
    bool get_is_running();
    void set_is_running(bool value);

private:
	mutex is_running_mutex;
};
