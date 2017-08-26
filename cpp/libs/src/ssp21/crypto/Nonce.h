
#ifndef SSP21_NONCE_H
#define SSP21_NONCE_H

#include "openpal/util/Uncopyable.h"

#include <cstdint>

namespace ssp21
{
    class Nonce final : private openpal::Uncopyable
    {

    public:

        void increment()
        {
            ++value;
        }

        void set(uint16_t new_value)
        {
            value = new_value;
        }

        inline uint16_t get() const
        {
            return value;
        }

		bool is_zero() const
		{
			return this->value == 0;
		}

    private:

        uint16_t value = 0;

    };

}

#endif
