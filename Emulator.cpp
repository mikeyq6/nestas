#include "inc/Emulator.h"

Emulator::Emulator() {
    shared_data = new SharedData();
    display = DisplayFactory::get_display(SDL, shared_data);
    cpu = new CPU(shared_data);
    ppu = new PPU(shared_data);
}
Emulator::~Emulator() {
    delete shared_data;
    delete display;
    delete cpu;
    delete ppu;
}

void Emulator::init() {
    cpu->init();
    ppu->init();
    display->init();
}

void Emulator::run() {
    thread cpu_thread{[this](){ this->run_cpu(); }};
    thread ppu_thread{[this](){ this->run_ppu(); }};

    run_display();
    
    cpu_thread.join();
    ppu_thread.join();
}

void Emulator::run_cpu() {
    cpu->run();
}

void Emulator::run_ppu() {
    ppu->run();
}

void Emulator::run_display() {
    display->draw();
    
    shared_data->set_is_running(false);
}

void Emulator::set_program_bytes(const char *bytes, int size) {
    for(int i = 0; i < size; i++) {
        // cpu->memory[i] = bytes[i];
    }
}