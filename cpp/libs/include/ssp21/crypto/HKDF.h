
#ifndef SSP21_HKDF_H
#define SSP21_HKDF_H

#include "CryptoTypedefs.h"

namespace ssp21
{
/**
* An implementation of HKDF based on some arbitrary hmac_func_t
*
* This is provided in the event that backends have the hash/hmac functions
* but do not implement HKDF internally.
*
*/
void hkdf(
    hmac_func_t hmac,
    const openpal::RSlice& chaining_key,
    std::initializer_list<openpal::RSlice> input_key_material,
    SymmetricKey& key1,
    SymmetricKey& key2
);

}

#endif
