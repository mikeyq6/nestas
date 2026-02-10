#pragma once

#include <vector>

#include "Display.h"

using std::fill;

class SDLDisplay : public Display
{
public:
    SDLDisplay(SharedData *shared_data);
    ~SDLDisplay();

    void draw() override;
    void init() override;
};