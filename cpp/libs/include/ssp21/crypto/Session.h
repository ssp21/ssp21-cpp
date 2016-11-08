
#ifndef SSP21_SESSION_H
#define SSP21_SESSION_H

#include "openpal/util/Uncopyable.h"
#include "openpal/logging/Logger.h"
#include "openpal/executor/Timestamp.h"
#include "openpal/container/Buffer.h"

#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/SessionModes.h"
#include "ssp21/crypto/Algorithms.h"

#include "ssp21/crypto/gen/UnconfirmedSessionData.h"

namespace ssp21
{
    class Session : private openpal::Uncopyable
    {

    public:		

		Session(uint16_t max_rx_payload_size);

		void initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start, const SessionKeys& keys);

		void reset();

		openpal::RSlice validate_user_data(const UnconfirmedSessionData& message, const openpal::Timestamp& now, std::error_code& ec);

    private:		
		
		openpal::Buffer rx_auth_buffer;

		bool valid = false;
        uint16_t rx_nonce = 0;
        uint16_t tx_nonce = 0;		

        SessionKeys keys;
        Algorithms::Session algorithms;
        openpal::Timestamp session_start = openpal::Timestamp::min_value();

    };

}

#endif
