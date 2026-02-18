#include "inc/Emulator.h"

Emulator::Emulator(const char *raw_cartridge_data) {
    this->raw_cartridge_data = raw_cartridge_data;
    
    shared_data = new SharedData();
    display = DisplayFactory::get_display(SDL, shared_data);
    mapper = MapperFactory::get_mapper(shared_data, raw_cartridge_data);
    cpu = new CPU(shared_data, mapper);
    ppu = new PPU(shared_data);
}
Emulator::~Emulator() {
    delete shared_data;
    delete display;
    delete cpu;
    delete ppu;
    delete mapper;
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
        raw_cartridge_data = bytes;
        // cpu->memory[i] = bytes[i];
    }
}