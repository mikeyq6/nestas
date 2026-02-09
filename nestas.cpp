#include "inc/nestas.h"

int main(int argc, char** argv) {
    std::cout << "Nestas Emulator" << std::endl;

    Emulator *emulator = new Emulator();
    emulator->Init();

    return 0;
}
