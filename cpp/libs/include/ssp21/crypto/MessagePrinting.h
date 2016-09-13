#ifndef SSP21_MESSAGE_PRINTING_H
#define SSP21_MESSAGE_PRINTING_H

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
            value.print(name, printer);
            return print_fields(printer, args...);
        }

	private:

		static void print_fields(IMessagePrinter& printer) {}
    };
}

#endif