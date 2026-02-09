#pragma once

#include "Display.h"

typedef enum _displayType {
    SDL
} DisplayType;

class DisplayFactory
{
public:
    static Display* GetDisplay(DisplayType displayType);
};