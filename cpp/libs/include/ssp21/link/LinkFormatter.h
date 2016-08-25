
#ifndef SSP21_LINKFORMATTER_H
#define SSP21_LINKFORMATTER_H

#include "openpal/util/Uncopyable.h"

#include "openpal/container/RSlice.h"
#include "openpal/container/WSlice.h"

#include "ssp21/LayerInterfaces.h"

namespace ssp21
{
    class LinkFormatter : private openpal::StaticOnly
    {
    public:

        // returns an empty slice if there wasn't sufficient space to write the frame
        static openpal::RSlice write(openpal::WSlice dest, const Message& message);
    };
}

#endif
