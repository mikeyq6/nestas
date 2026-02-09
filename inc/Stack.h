#pragma once

#include <vector>

constexpr auto STACK_SIZE = 4096;

template <typename T>
class Stack {
private:
    T *stack;
    uint16_t sp;

public:
    Stack();
    ~Stack();

    void push(T val);
    T pop();
};

#include "Stack.tpp"