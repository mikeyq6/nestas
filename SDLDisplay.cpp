#include "inc/SDLDisplay.h"

SDLDisplay::SDLDisplay(SharedData *shared_data) : Display(shared_data) {

}
SDLDisplay::~SDLDisplay() {

}

void SDLDisplay::init() {
    fill(pixels, pixels + NUM_PIXELS, 0);
}

void SDLDisplay::draw() {
    return;
}