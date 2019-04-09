
#ifndef SSP21_NONCE_FUNCTIONS_H
#define SSP21_NONCE_FUNCTIONS_H

#include "ser4cpp/util/Uncopyable.h"
#include "ser4cpp/serialization/BigEndian.h"

#include "ssp21/crypto/gen/NonceMode.h"

#include <cstdint>

namespace ssp21
{
    using verify_nonce_func_t = bool(*)(uint16_t last_nonce, uint16_t new_nonce);

    struct NonceFunctions : private ser4cpp::StaticOnly
    {
        inline static verify_nonce_func_t default_verify()
        {
            return &verify_strict_increment;
        }

        inline static bool verify_strict_increment(uint16_t last_nonce, uint16_t new_nonce)
        {
            if (last_nonce == ser4cpp::UInt16::max_value) // don't allow rollover
            {
                return false;
            }

            return new_nonce == (last_nonce + 1);
        }

        inline static bool verify_greater_than_last(uint16_t last_nonce, uint16_t new_nonce)
        {
            return new_nonce > last_nonce;
        }

        inline static bool equal_to_zero(uint16_t last_nonce, uint16_t new_nonce)
        {
            return (last_nonce == 0) && (new_nonce == 0);
        }

    };

}

#endif
