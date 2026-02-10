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
    void set_program_bytes(const char *bytes, int size);

private:
    Display* display;
    CPU* cpu;
    SharedData *shared_data;
};