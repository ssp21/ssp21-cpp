

#include "ssp21/crypto/Reassembler.h"

#include "ssp21/crypto/gen/CryptoError.h"

using namespace openpal;

namespace ssp21
{

    Reassembler::Reassembler(uint16_t max_reassembled_size) :
        buffer(max_reassembled_size)
    {}


    ReassemblyResult Reassembler::process(bool fir, bool fin, uint32_t nonce, const openpal::RSlice& data)
    {
        return ReassemblyResult::bad_nonce;
    }

}

