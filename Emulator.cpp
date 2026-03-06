#include "inc/Emulator.h"

Emulator::Emulator(const char *raw_cartridge_data) {
    this->raw_cartridge_data = raw_cartridge_data;
    
    shared_data = new SharedData();
    display = DisplayFactory::get_display(SDL, shared_data);
    mapper = MapperFactory::get_mapper(shared_data, raw_cartridge_data);
    mapper_for_ppu = MapperFactory::get_mapper(shared_data, raw_cartridge_data); // it's all static data, so create a copy for PPU rather than share between threads
    ppu = new PPU(shared_data, mapper_for_ppu);
    cpu = new CPU(shared_data, ppu, mapper);
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

    run_display();
    
    cpu_thread.join();
}

void Emulator::run_cpu() {
    cpu->run();
}

void Emulator::run_display() {
    display->draw();
    
    shared_data->set_is_running(false);
}