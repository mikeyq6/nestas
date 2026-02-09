#include "inc/Emulator.h"

Emulator::Emulator() {
    display = DisplayFactory::GetDisplay(SDL);
}
Emulator::~Emulator() {

}

void Emulator::Init() {

}