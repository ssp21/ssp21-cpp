
#include "Program.h"

#include <iostream>

int main(int argc, char* argv[])
{
    try {
        Program program;
        program.run(argc, argv);
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}
