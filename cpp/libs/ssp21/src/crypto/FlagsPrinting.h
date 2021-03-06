#ifndef SSP21_FLAGS_PRINTING_H
#define SSP21_FLAGS_PRINTING_H

#include "ssp21/crypto/IMessagePrinter.h"

#include "ser4cpp/util/Uncopyable.h"

namespace ssp21 {

class FlagsPrinting : private ser4cpp::StaticOnly {

public:
    // simple for now, but we can later use variadic template
    static void print(IMessagePrinter& printer, const char* bitfield_name, const char* name1, bool value1, const char* name2, bool value2);

private:
    static const int max_line_size = 80;
};
}

#endif