
#ifndef SSP21_IWRITABLE_H
#define SSP21_IWRITABLE_H

#include "ssp21/gen/FormatError.h"
#include "openpal/container/WSlice.h"

namespace ssp21
{

    /**
    * Any object that can be serialized to a slice
    */
    class IWritable
    {

    public:

        /// Advances the output. Returns an error if writing fails.
        virtual FormatError write(openpal::WSlice& output) const = 0;
    };

}

#endif
