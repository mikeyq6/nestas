#pragma once

#include "Display.h"

typedef enum _displayType {
    SDL
} DisplayType;

class DisplayFactory
{
public:
    static Display* get_display(DisplayType displayType, SharedData *shared_data);
};