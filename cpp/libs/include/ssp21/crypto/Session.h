
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
		friend class Handshake;

    public:

	

	private:

		
		SessionKeys keys_;
		Algorithms::Session algorithms_;
		openpal::Timestamp session_start_ = openpal::Timestamp::min_value();

    };

}

#endif
