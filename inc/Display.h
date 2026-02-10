#pragma once

#include "SharedData.h"

class Display {
public:
    Display(SharedData *shared_data);
    virtual ~Display() = default;
    virtual void draw() = 0;
    
    virtual void init() = 0;

protected:
    SharedData *shared_data;
};