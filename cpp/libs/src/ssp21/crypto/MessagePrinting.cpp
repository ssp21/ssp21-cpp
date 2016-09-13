
#include "ssp21/crypto/MessagePrinting.h"

#include "openpal/logging/LogMacros.h"

#include "openpal/util/ToHex.h"

using namespace openpal;

namespace ssp21
{

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

