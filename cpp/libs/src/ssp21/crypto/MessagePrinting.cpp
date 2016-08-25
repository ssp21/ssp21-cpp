
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

// enums
void MessagePrinting::print(IMessagePrinter& printer, const char* name, Function value)
{
    print_any_enum<max_line_size, FunctionSpec>(printer, name, value);
}

void MessagePrinting::print(IMessagePrinter& printer, const char* name, CertificateMode value)
{
    print_any_enum<max_line_size, CertificateModeSpec>(printer, name, value);
}

void MessagePrinting::print(IMessagePrinter& printer, const char* name, DHMode value)
{
    print_any_enum<max_line_size, DHModeSpec>(printer, name, value);
}

void MessagePrinting::print(IMessagePrinter& printer, const char* name, HandshakeError value)
{
    print_any_enum<max_line_size, HandshakeErrorSpec>(printer, name, value);
}

void MessagePrinting::print(IMessagePrinter& printer, const char* name, NonceMode value)
{
    print_any_enum<max_line_size, NonceModeSpec>(printer, name, value);
}

void MessagePrinting::print(IMessagePrinter& printer, const char* name, SessionMode value)
{
    print_any_enum<max_line_size, SessionModeSpec>(printer, name, value);
}

void MessagePrinting::print(IMessagePrinter& printer, const char* name, HashMode value)
{
    print_any_enum<max_line_size, HashModeSpec>(printer, name, value);
}

// sequences
void MessagePrinting::print(IMessagePrinter& printer, const char* name, const Seq8& value)
{
    printer.print(name, value);
}

void MessagePrinting::print(IMessagePrinter& printer, const char* name, const Seq16& value)
{
    printer.print(name, value);
}

void MessagePrinting::print(IMessagePrinter& printer, const char* name, const Seq8Seq16& value)
{
    printer.print(name, value);
}

}

