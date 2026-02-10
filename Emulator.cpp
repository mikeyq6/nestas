#include "inc/Emulator.h"

Emulator::Emulator() {
    shared_data = new SharedData();
    display = DisplayFactory::get_display(SDL, shared_data);
    cpu = new CPU(shared_data);
}
Emulator::~Emulator() {
    delete shared_data;
    delete display;
    delete cpu;
}

void Emulator::init() {
    cpu->init();
    display->init();
}

void Emulator::run() {
    std::thread cpu_thread{[this](){ this->run_cpu(); }};

    run_display();
    
    cpu_thread.join();
}

void Emulator::run_cpu() {
    while(shared_data->get_is_running()) {
        cpu->run();
    }
}

void Emulator::run_display() {
    display->Draw();
    
    shared_data->set_is_running(false);
}

void Emulator::set_program_bytes(const char *bytes, int size) {
    for(int i = 0; i < size; i++) {
        // cpu->memory[i] = bytes[i];
    }
}