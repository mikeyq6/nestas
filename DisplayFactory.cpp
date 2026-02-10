#include "inc/DisplayFactory.h"

#include "inc/Display.h"
#include "inc/SDLDisplay.h"

Display* DisplayFactory::get_display(DisplayType displayType, SharedData *shared_data) {
    switch(displayType) {
        case SDL:
            return new SDLDisplay(shared_data);
            break;
        default:
            return new SDLDisplay(shared_data);
            break;
    }
}