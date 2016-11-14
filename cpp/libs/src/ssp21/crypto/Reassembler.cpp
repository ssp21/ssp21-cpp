

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
        this->last_nonce = nonce;

        if (fir)
        {
            this->reassembled_data = data.copy_to(this->buffer.as_wslice());
            return fin ? ReassemblyResult::complete : ReassemblyResult::partial;
        }
        else
        {
            if (this->reassembled_data.is_empty())
            {
                this->reset();
                return ReassemblyResult::no_prior_fir;
            }

            if ((nonce == 0) || ((nonce - 1) != this->last_nonce))
            {
                this->reset();
                return ReassemblyResult::bad_nonce;
            }

            const auto current_length = this->reassembled_data.length();

            auto remainder = this->buffer.as_wslice().skip(current_length);

            if (data.length() > remainder.length())
            {
                this->reset();
                return ReassemblyResult::overflow;
            }

            data.copy_to(remainder);
            this->reassembled_data = this->buffer.as_rslice().take(current_length + data.length());

            return fin ? ReassemblyResult::complete : ReassemblyResult::partial;
        }
    }

}

