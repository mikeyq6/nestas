#pragma once

class Display {
public:
    virtual ~Display() = default;
    virtual void Draw() = 0;
};