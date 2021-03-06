
#include "crypto/FlagsPrinting.h"

#include "log4cpp/LogMacros.h"

namespace ssp21 {
void FlagsPrinting::print(IMessagePrinter& printer, const char* bitfield_name, const char* name1, bool value1, const char* name2, bool value2)
{
    char message[max_line_size];
    SAFE_STRING_FORMAT(message, max_line_size, "%s { %s: %d %s: %d }", bitfield_name, name1, value1, name2, value2);
    printer.print(message);
}
}
