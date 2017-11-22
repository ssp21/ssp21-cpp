
#include "qix/QIXKeySource.h"

#include "ssp21/util/ConsolePrettyPrinter.h"
#include "ssp21/stack/LogLevels.h"

#include "openpal/logging/LogMacros.h"

#include <iostream>

using namespace ssp21;

int main(int argc, char*  argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: " << std::endl;
		std::cerr << "qixreader <serial device>" << std::endl;
		return -1;
	}	

    try
    {
		QIXKeySource key_source(
			nullptr, 
			openpal::Logger(std::make_shared<ssp21::ConsolePrettyPrinter>(), Module::id, "qix-reader", openpal::LogLevels(~0)),
			argv[1]
		);

		std::cout << "press <enter> to terminate" << std::endl;
		std::cin.ignore();
    }
    catch (const std::exception& ex)
    {				
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}

