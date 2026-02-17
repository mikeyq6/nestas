#include "inc/PPU.h"

PPU::PPU(SharedData *shared_data) {
    this->shared_data = shared_data;
}
PPU::~PPU() {

}

void PPU::init() {
    for(int i=0; i<0x8; i++) {
        shared_data->set_ppu_register(i, 0);
    }
}