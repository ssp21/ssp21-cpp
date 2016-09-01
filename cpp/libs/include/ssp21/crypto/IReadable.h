
#ifndef SSP21_IREADABLE_H
#define SSP21_IREADABLE_H

#include "ssp21/gen/ParseError.h"

#include "openpal/container/RSlice.h"

namespace ssp21
{

    /**
    * Any object that can be deserialized from a byte array slice
    */
    class IReadable
    {

    public:

        /// consumes the input, returns an error if parsing fails
        virtual ParseError read(openpal::RSlice& input) = 0;

    };

}

#endif
