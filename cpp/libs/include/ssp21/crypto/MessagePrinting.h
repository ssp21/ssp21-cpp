#ifndef SSP21_MESSAGE_PRINTING_H
#define SSP21_MESSAGE_PRINTING_H

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

        // anything with a print method
		template <class T>
        static void print(IMessagePrinter& printer, const char* name, const T& field)
        {
            field.print(name, printer);
        }

    };
}

#endif