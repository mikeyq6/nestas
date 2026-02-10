#include "inc/Emulator.h"

Emulator::Emulator() {
    shared_data = new SharedData();
    display = DisplayFactory::GetDisplay(SDL);
    cpu = new CPU(shared_data);
}
Emulator::~Emulator() {
    delete shared_data;
    delete display;
    delete cpu;
}

void Emulator::Init() {
    cpu->init();
    display->init();
}

void Emulator::set_program_bytes(const char *bytes, int size) {
    for(int i = 0; i < size; i++) {
        // cpu->memory[i] = bytes[i];
    }
}