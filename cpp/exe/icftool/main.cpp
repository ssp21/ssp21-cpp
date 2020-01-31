
#include "Program.h"

#include <iostream>

#include <sodium/Backend.h>

int main(int argc, char* argv[])
{
    ssp21::sodium::initialize();

    try {
        Program program;
        program.run(argc, argv);
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}
