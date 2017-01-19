#ifndef SSP21_CHANNEL_H
#define SSP21_CHANNEL_H

#include "openpal/util/Uncopyable.h"

#include "ssp21/SequenceTypes.h"
#include "ssp21/LayerInterfaces.h"

#include <stdexcept>

namespace ssp21
{
    // buffers messages from a transmitter to a receiver
    class Channel : private openpal::Uncopyable
    {

    public:

        void transmit(const seq32_t& data)
        {
            if (slice.is_not_empty())
            {
                throw std::logic_error("already transmitting");
            }

            slice = data;

            // TODO - notify
        }

        bool receive(IUpperLayer& upper)
        {
            if (slice.is_empty()) return false;

            if(upper.on_rx_ready(slice))
            {
                slice.make_empty();

                // TODO - notify

                return true;
            }
        }

    private:

        seq32_t slice;
    };

}

#endif
