#pragma once

#include "Constants.h"
#include "SharedData.h"

class Display {
public:
    Display(SharedData *shared_data);
    virtual ~Display() = default;

    virtual void init() = 0;
    virtual void draw() = 0;

protected:
    SharedData *shared_data;
    uint32_t pixels[NUM_PIXELS];
};