#pragma once

#include "CPU.h"
#include "Display.h"
#include "DisplayFactory.h"
#include "SharedData.h"

class Emulator {
public:
    Emulator();
    ~Emulator();

    void Init();

private:
    Display* display;
    CPU* cpu;
    SharedData *shared_data;
};