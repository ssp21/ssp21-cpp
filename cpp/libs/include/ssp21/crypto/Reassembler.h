
#ifndef SSP21_REASSEMBLER_H
#define SSP21_REASSEMBLER_H

#include "openpal/util/Uncopyable.h"
#include "openpal/executor/Timestamp.h"
#include "openpal/container/Buffer.h"

#include "ssp21/crypto/gen/ReassemblyResult.h"

#include <system_error>

namespace ssp21
{
    class Reassembler : private openpal::Uncopyable
    {

    public:

        explicit Reassembler(uint16_t max_reassembled_size);

        ReassemblyResult process(bool fir, bool fin, uint32_t nonce, const openpal::RSlice& data);

        void reset()
        {
            this->last_nonce = 0;
            this->reassembled_data.make_empty();
        }

        bool has_data() const
        {
            return reassembled_data.is_not_empty();
        }

        openpal::RSlice get_data() const
        {
            return reassembled_data;
        }

    private:

        uint32_t last_nonce = 0;

        openpal::RSlice reassembled_data;

        openpal::Buffer buffer;

    };

}

#endif
