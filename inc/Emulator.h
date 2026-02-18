#pragma once

#include <thread>

#include "CPU.h"
#include "Display.h"
#include "DisplayFactory.h"
#include "mappers/Mapper.h"
#include "MapperFactory.h"
#include "PPU.h"
#include "SharedData.h"

using std::thread;

class Emulator {
public:
    Emulator(const char *raw_cartridge_data);
    ~Emulator();

    void init();
    void run();
    void set_program_bytes(const char *bytes, int size);

private:
    Display* display;
    CPU* cpu;
    PPU *ppu;
    SharedData *shared_data;
    Mapper *mapper;
    const char *raw_cartridge_data;

    void run_cpu();
    void run_display();
    void run_ppu();
};