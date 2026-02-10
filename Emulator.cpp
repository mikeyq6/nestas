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