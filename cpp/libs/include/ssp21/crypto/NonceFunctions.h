
#ifndef SSP21_NONCE_FUNCTIONS_H
#define SSP21_NONCE_FUNCTIONS_H

#include "openpal/util/Uncopyable.h"
#include "openpal/util/Limits.h"

#include "ssp21/gen/NonceMode.h"

#include <cstdint>

namespace ssp21
{
    typedef bool(*verify_nonce_func_t)(uint32_t last_nonce, uint32_t new_nonce);

    struct NonceFunctions : private openpal::StaticOnly
    {
        inline static verify_nonce_func_t default_verify()
        {
            return &verify_strict_increment;
        }

        inline static bool verify_strict_increment(uint32_t last_nonce, uint32_t new_nonce)
        {
            if (last_nonce == openpal::max_value<uint32_t>()) // don't allow rollover below
            {
                return false;
            }

            return new_nonce == (last_nonce + 1);
        }

        inline static bool verify_greater_than_last(uint32_t last_nonce, uint32_t new_nonce)
        {
            return new_nonce > last_nonce;
        }

    };

}

#endif
