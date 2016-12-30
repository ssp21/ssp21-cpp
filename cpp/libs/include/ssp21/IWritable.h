
#ifndef SSP21_IWRITABLE_H
#define SSP21_IWRITABLE_H

#include "ssp21/crypto/FormatResult.h"

namespace ssp21
{
    /**
    * something that can be written to an output buffer
    */
    struct IWritable
    {            

        virtual FormatResult write(wseq32_t& output) const = 0;      

    };

}

#endif
