
#ifndef SSP21_IMESSAGE_H
#define SSP21_IMESSAGE_H

#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21
{
    /**
    * Messages can be printed
    */
    class IMessage
    {

    public:

        virtual void print(IMessagePrinter& printer) const = 0;

    };

}

#endif
