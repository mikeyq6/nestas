#include "inc/nestas.h"

using std::ifstream;
using std::string;

int main(int argc, char** argv) {
    std::cout << "Nestas Emulator" << std::endl;

    if(argc < 2) {
        std::cout << "Usage: nestas <ROM file>" << std::endl;
        return 1;
    }

    string rom_file = argv[argc-1];
    if(!read_file_contents(rom_file)) {
        std::cout << "Unable to open file " << rom_file << std::endl;
        return(2);
    }

    emulator = new Emulator(reinterpret_cast<char*>(buffer.data()));
    emulator->init();

    emulator->run();

    return 0;
}


bool read_file_contents(string filename) {
    ifstream input(filename, std::ios::binary);
    if (input.is_open()) {
        input.seekg(0, std::ios::end);
        int size = input.tellg();
        input.seekg(0, std::ios::beg);

        buffer = vector<byte>(size);
        input.read(reinterpret_cast<char*>(buffer.data()), size);
        input.close();
    } else {
        return false;
    } 
    return true;
}