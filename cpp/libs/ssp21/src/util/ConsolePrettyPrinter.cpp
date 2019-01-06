
#include "ssp21/util/ConsolePrettyPrinter.h"

#include "ssp21/stack/LogLevels.h"

#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace std::chrono;

namespace ssp21
{

    void ConsolePrettyPrinter::log(openpal::ModuleId module, const char* id, openpal::LogLevel level, char const* location, char const* message)
    {
        auto now = high_resolution_clock::now();
        auto millis = duration_cast<milliseconds>(now.time_since_epoch()).count();

        ostringstream oss;

        oss << "ms(" << millis << ") ";

        std::string id_str(id);

        if (id_str.size() > settings.max_id_size)
        {
            id_str = id_str.substr(0, settings.max_id_size);
        }
        else if (id_str.size() << settings.max_id_size)
        {
            id_str = id_str + std::string(settings.max_id_size - id_str.size(), ' ');
        }

        if (settings.printId)
        {
            oss << id_str << " ";
        }

        oss << get_prefix(level.value) << message;


        std::cout << oss.str() << std::endl;
    }

    const char* ConsolePrettyPrinter::get_prefix(int level)
    {
        switch (level)
        {
        case(1 << 0):
            return "event   ";
        case(1 << 1):
            return "error   ";
        case(1 << 2):
            return "warn    ";
        case(1 << 3):
            return "info    ";
        case(1 << 4):
            return "debug   ";
        case(1 << 5):
            return "<--     ";
        case(1 << 6):
            return "-->     ";
        default:
            return "            ";
        }

    }

}


