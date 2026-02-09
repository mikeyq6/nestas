#pragma once

#include <cstdint>

#include "Stack.h"

using std::uint16_t;

class CPU {
private:
    CPU();
    ~CPU();

    Stack<uint16_t> *stack;
};