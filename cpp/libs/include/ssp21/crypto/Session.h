
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

        template <class InitKeysFun>
        void initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start, const InitKeysFun& init_keys)
        {
            this->initialize(algorithms, session_start);

            init_keys(this->keys);
        }

    private:

        void initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start)
        {
            this->rx_nonce = this->tx_nonce = 0;
            this->algorithms = algorithms;
            this->session_start = session_start;
        }

        uint16_t rx_nonce = 0;
        uint16_t tx_nonce = 0;

        SessionKeys keys;
        Algorithms::Session algorithms;
        openpal::Timestamp session_start = openpal::Timestamp::min_value();

    };

}

#endif
