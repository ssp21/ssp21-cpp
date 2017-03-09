
#ifndef SSP21_HKDF_H
#define SSP21_HKDF_H

#include "ssp21/crypto/CryptoTypedefs.h"

namespace ssp21
{
    /**
    * An implementation of HKDF based on some arbitrary mac_func_t
    *
    * This is provided in the event that backends have the hash/hmac functions
    * but do not implement HKDF internally.
    *
    */
    template <mac_func_t mac_func>
    void hkdf(
        const seq8_t& salt,
        std::initializer_list<seq32_t> input_key_material,
        SymmetricKey& output1,
        SymmetricKey& output2)
    {
        // extract
        HashOutput temp_key;
        mac_func(salt, input_key_material, temp_key);

        const uint8_t ONE = 0x01;
        const uint8_t TWO = 0x02;

        // expand
        mac_func(temp_key.as_seq(), { seq32_t(&ONE, 1) }, output1);
        mac_func(temp_key.as_seq(), { output1.as_seq(), seq32_t(&TWO, 1) }, output2);

        // this will truncate the lengths in the event that the hmac-output length_ is > the symmetric key length_ we need
        // TODO: research how noise implementations handle this
        // Should we always have 256-bit keys?
        output1.set_type(BufferType::symmetric_key);
        output2.set_type(BufferType::symmetric_key);
    }

}

#endif
