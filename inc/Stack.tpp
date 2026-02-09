template <typename T>
Stack<T>::Stack() {
    sp = STACK_SIZE - 1;
    stack = new T[STACK_SIZE];
    std::fill(stack, stack + STACK_SIZE, 0);
}
template <typename T>
Stack<T>::~Stack() {
    delete[] stack;
}

template <typename T>
void Stack<T>::push(T val) {
    if(sp > 0) {
        stack[sp--] = val;
    }
    // TODO: Throw exception if stack full
}

template <typename T>
T Stack<T>::pop() {
    if(sp < STACK_SIZE - 1) {
        return stack[++sp];
    }
    // TODO: Throw exception
    return 0;
}