
#ifndef SSP21_SESSION_H
#define SSP21_SESSION_H

#include "openpal/util/Uncopyable.h"
#include "openpal/executor/Timestamp.h"

#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/SessionModes.h"
#include "ssp21/crypto/Algorithms.h"

namespace ssp21
{
    class Session : private openpal::Uncopyable
    {

    public:

        template <class KeyFun>
        void initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start, const KeyFun& init_keys)
        {
            this->initialize(algorithms, session_start);

            init_keys(keys_);
        }

    private:

        void initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start)
        {
            this->rx_nonce = this->tx_nonce = 0;
            this->algorithms_ = algorithms;
            this->session_start_ = session_start;
        }

        uint16_t rx_nonce = 0;
        uint16_t tx_nonce = 0;

        SessionKeys keys_;
        Algorithms::Session algorithms_;
        openpal::Timestamp session_start_ = openpal::Timestamp::min_value();

    };

}

#endif
