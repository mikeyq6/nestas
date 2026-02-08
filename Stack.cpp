#include "inc/Stack.h"

Stack::Stack() {
    sp = STACK_SIZE - 1;
    stack = new uint16_t[STACK_SIZE];
    std::fill(stack, stack + STACK_SIZE, 0);
}
Stack::~Stack() {
    delete[] stack;
}

void Stack::push(uint16_t val) {
    if(sp > 0) {
        stack[sp--] = val;
    }
    // TODO: Throw exception if stack full
}
uint16_t Stack::pop() {
    if(sp < STACK_SIZE - 1) {
        return stack[++sp];
    }
    // TODO: Throw exception
    return 0;
}