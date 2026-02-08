#pragma once

#include <cstdint>
#include <vector>

constexpr auto STACK_SIZE = 4096;

using std::uint16_t;

class Stack {
private:
    uint16_t *stack;
    uint16_t sp;

public:
    Stack();
    ~Stack();

    void push(uint16_t val);
    uint16_t pop();
};