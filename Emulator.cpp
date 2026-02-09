#include "inc/Emulator.h"

Emulator::Emulator() {
    display = DisplayFactory::GetDisplay(SDL);
}
Emulator::~Emulator() {
    delete display;
}

void Emulator::Init() {

}