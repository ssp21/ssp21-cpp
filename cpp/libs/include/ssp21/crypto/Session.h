
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
        
		void initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start, const SessionKeys& keys);

		void close();

    private:		

		bool valid = false;
        uint16_t rx_nonce = 0;
        uint16_t tx_nonce = 0;		

        SessionKeys keys;
        Algorithms::Session algorithms;
        openpal::Timestamp session_start = openpal::Timestamp::min_value();

    };

}

#endif
