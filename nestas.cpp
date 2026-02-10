#include "inc/nestas.h"

using std::ifstream;
using std::string;

int main(int argc, char** argv) {
    std::cout << "Nestas Emulator" << std::endl;

    if(argc < 2) {
        std::cout << "Usage: nestas <ROM file>" << std::endl;
        return 1;
    }

    emulator = new Emulator();
    emulator->Init();

    string rom_file = argv[argc-1];
    if(!read_file_contents(rom_file)) {
        std::cout << "Unable to open file " << rom_file << std::endl;
        return(2);
    }

    return 0;
}


bool read_file_contents(string filename) {
    ifstream input(filename, std::ios::binary);
    if (input.is_open()) {
        input.seekg(0, std::ios::end);
        int size = input.tellg();
        input.seekg(0, std::ios::beg);

        vector<byte> buffer(size);
        input.read(reinterpret_cast<char*>(buffer.data()), size);
        input.close();

        emulator->set_program_bytes(reinterpret_cast<char*>(buffer.data()), size); 
    } else {
        return false;
    } 
    return true;
}