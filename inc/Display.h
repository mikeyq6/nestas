#pragma once

class Display {
public:
    virtual ~Display() = default;
    virtual void Draw() = 0;
    
    virtual void init() = 0;
};