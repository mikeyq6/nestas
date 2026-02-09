#include "inc/CPU.h"

CPU::CPU() {
    stack = new Stack<uint16_t>();
}
CPU::~CPU() {
    delete stack;
}