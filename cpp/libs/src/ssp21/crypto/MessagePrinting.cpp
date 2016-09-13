
#include "ssp21/crypto/MessagePrinting.h"

#include "openpal/logging/LogMacros.h"

#include "openpal/util/ToHex.h"

using namespace openpal;

namespace ssp21
{


    template <int size, class EnumSpec>
    void print_any_enum(IMessagePrinter& printer, const char* name, typename EnumSpec::enum_type_t value)
    {
        printer.print(name, EnumSpec::to_string(value));
    }


    // integers
    void MessagePrinting::print(IMessagePrinter& printer, const char* name, uint8_t value)
    {
        printer.print(name, value);
    }

    void MessagePrinting::print(IMessagePrinter& printer, const char* name, uint16_t value)
    {
        printer.print(name, value);
    }
    void MessagePrinting::print(IMessagePrinter& printer, const char* name, uint32_t value)
    {
        printer.print(name, value);
    }

}

