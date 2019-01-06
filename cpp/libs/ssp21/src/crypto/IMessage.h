
#ifndef SSP21_IMESSAGE_H
#define SSP21_IMESSAGE_H

#include "IWritable.h"
#include "crypto/gen/ParseError.h"

namespace ssp21
{
    /**
    * Messages can be read, written, and printed
    */
    class IMessage : public IWritable
    {

    public:

        virtual ParseError read(seq32_t input) = 0;

    };

}

#endif
