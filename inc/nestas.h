#include <iostream>

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "Emulator.h"

using std::byte;
using std::string;
using std::vector;

Emulator *emulator = nullptr;

bool read_file_contents(string filename);