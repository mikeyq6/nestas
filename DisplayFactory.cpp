#include "inc/DisplayFactory.h"

#include "inc/Display.h"
#include "inc/SDLDisplay.h"

Display* DisplayFactory::GetDisplay(DisplayType displayType) {
    switch(displayType) {
        case SDL:
            return new SDLDisplay();
            break;
        default:
            return new SDLDisplay();
            break;
    }
}