#include "inc/PPU.h"

PPU::PPU() {

}
PPU::~PPU() {

}

void PPU::init() {
    ppuctrl = 0;
    ppumask = 0;
    ppustatus = 0;
    oamaddr = 0;
    oamdata = 0;
    ppuscroll = 0;
    ppuaddr = 0;
    ppudata = 0;
}