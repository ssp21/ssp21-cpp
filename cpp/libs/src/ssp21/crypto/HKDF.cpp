
#include "ssp21/crypto/HKDF.h"

namespace ssp21
{

void hkdf(
    hmac_func_t hmac,
    const openpal::RSlice& chaining_key,
    std::initializer_list<openpal::RSlice> input_key_material,
    SymmetricKey& output1,
    SymmetricKey& output2)
{
    // extract
    HashOutput temp_key;
    hmac(chaining_key, input_key_material, temp_key);

    const uint8_t ONE = 0x01;
    const uint8_t TWO = 0x02;

    // expand
    hmac(temp_key.as_slice(), { openpal::RSlice(&ONE, 1) }, output1);
    hmac(temp_key.as_slice(), { output1.as_slice(), openpal::RSlice(&TWO, 1) }, output2);

    // this will truncate the lengths in the event that the hmac-output length_ is > the symmetric key length_ we need
    // TODO: research how noise implementations handle this
    // Should we always have 256-bit keys?
    output1.set_type(BufferType::symmetric_key);
    output2.set_type(BufferType::symmetric_key);
}

}


