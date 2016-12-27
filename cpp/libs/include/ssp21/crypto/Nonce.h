
#ifndef SSP21_NONCE_H
#define SSP21_NONCE_H

#include "openpal/util/Uncopyable.h"

#include <cstdint>
#include <limits>

namespace ssp21
{
    class Nonce final : private openpal::Uncopyable
    {

    public:

        inline bool is_max_value() const
        {
            return value == std::numeric_limits<uint16_t>::max();
        }

        bool increment()
        {
            if (is_max_value()) return false;
            else
            {
                ++value;
                return true;
            }
        }

        void set(uint16_t new_value)
        {
            value = new_value;
        }

        inline uint16_t get() const
        {
            return value;
        }

    private:

        uint16_t value = 0;

    };

}

#endif
