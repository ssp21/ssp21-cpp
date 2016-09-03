
#ifndef SSP21_IMESSAGEFIELD_H
#define SSP21_IMESSAGEFIELD_H

#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"

#include "ssp21/crypto/IMessagePrinter.h"

#include "openpal/container/RSlice.h"

namespace ssp21
{

    /**
    * Message fields can be read, written, and printed
    */
    class IMessageField
    {

    public:

        /// consumes the input, returns an error if parsing fails
        virtual ParseError read(openpal::RSlice& input) = 0;
        virtual FormatError write(openpal::WSlice& output) const = 0;
        virtual void print(const char* name, IMessagePrinter& printer) const = 0;

    };

}

#endif
