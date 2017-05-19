
#ifndef SSP21_REASSEMBLER_H
#define SSP21_REASSEMBLER_H

#include "openpal/util/Uncopyable.h"
#include "openpal/executor/Timestamp.h"
#include "openpal/container/Buffer.h"

#include "ssp21/crypto/gen/ReassemblyResult.h"
#include "ssp21/util/SequenceTypes.h"

#include <system_error>

namespace ssp21
{
    class Reassembler : private openpal::Uncopyable
    {

    public:

        explicit Reassembler(uint16_t max_reassembled_size);

        ReassemblyResult process(bool fir, bool fin, uint32_t nonce, const seq32_t& data);

        void reset()
        {
            this->last_nonce = 0;
            this->reassembled_data.make_empty();
        }

        bool has_data() const
        {
            return reassembled_data.is_not_empty();
        }

        seq32_t get_data() const
        {
            return reassembled_data;
        }

    private:

        uint32_t last_nonce = 0;

        seq32_t reassembled_data;

        openpal::Buffer buffer;

    };

}

#endif
