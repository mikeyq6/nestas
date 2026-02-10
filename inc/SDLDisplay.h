#pragma once

#include "Display.h"

class SDLDisplay : public Display
{
public:
    SDLDisplay();
    ~SDLDisplay();

    void Draw() override;
    void init() override;
};