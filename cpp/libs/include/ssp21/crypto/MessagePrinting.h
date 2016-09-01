#ifndef SSP21_MESSAGE_PRINTING_H
#define SSP21_MESSAGE_PRINTING_H

#include "ssp21/gen/Function.h"
#include "ssp21/gen/CertificateMode.h"
#include "ssp21/gen/DHMode.h"
#include "ssp21/gen/HandshakeError.h"
#include "ssp21/gen/NonceMode.h"
#include "ssp21/gen/SessionMode.h"
#include "ssp21/gen/HashMode.h"

#include "ssp21/crypto/SequenceTypes.h"
#include "ssp21/crypto/IMessagePrinter.h"

#include "openpal/util/Uncopyable.h"

namespace ssp21
{

    class MessagePrinting : private openpal::StaticOnly
    {

    public:

        template <typename T, typename... Args>
        static void print_fields(IMessagePrinter& printer, const char* name, const T& value, Args& ... args)
        {
            print(printer, name, value);
            return print_fields(printer, args...);
        }

        static void print_fields(IMessagePrinter& printer) {}

    private:

        static const int max_line_size = 80;

        // integers
        static void print(IMessagePrinter& printer, const char* name, uint8_t value);
        static void print(IMessagePrinter& printer, const char* name, uint16_t value);
        static void print(IMessagePrinter& printer, const char* name, uint32_t value);

        // enums
        static void print(IMessagePrinter& printer, const char* name, Function value);
        static void print(IMessagePrinter& printer, const char* name, CertificateMode value);
        static void print(IMessagePrinter& printer, const char* name, DHMode value);
        static void print(IMessagePrinter& printer, const char* name, HandshakeError value);
        static void print(IMessagePrinter& printer, const char* name, NonceMode value);
        static void print(IMessagePrinter& printer, const char* name, SessionMode value);
        static void print(IMessagePrinter& printer, const char* name, HashMode value);

        // any printable
        static void print(IMessagePrinter& printer, const char* name, const IPrintable& value)
        {
            value.print(name, printer);
        }

    };
}

#endif