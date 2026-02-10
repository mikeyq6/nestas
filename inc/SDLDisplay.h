#pragma once

#include "Display.h"

class SDLDisplay : public Display
{
public:
    SDLDisplay(SharedData *shared_data);
    ~SDLDisplay();

    void Draw() override;
    void init() override;
};