
#ifndef SSP21_HKDF_H
#define SSP21_HKDF_H

#include "ssp21/crypto/CryptoTypedefs.h"

namespace ssp21 {
namespace sodium {

    /**
	* An implementation of HKDF based on some arbitrary mac_func_t
	*
	* Libsodium does not implement HKDF internally.
	*/
    template <mac_func_t mac_func>
    void hkdf(
        const seq32_t& salt,
        std::initializer_list<seq32_t> input_key_material,
        SymmetricKey& output1,
        SymmetricKey& output2)
    {
        // extract
        HashOutput temp_key;
        mac_func(salt, input_key_material, temp_key);

        const uint8_t values[] = { 0x01, 0x02 };

        // expand
        const std::initializer_list<seq32_t> input1 = { seq32_t(values, 1) };
        mac_func(temp_key.as_seq(), input1, output1);
        const std::initializer_list<seq32_t> input2 = { output1.as_seq(), seq32_t(values + 1, 1) };
        mac_func(temp_key.as_seq(), input2, output2);

        output1.set_length(BufferLength::length_32);
        output2.set_length(BufferLength::length_32);
    }
}

}

#endif
