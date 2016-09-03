
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

		SessionKeys& get_keys() { return keys_; }

		void initialize(const Algorithms::Session& algorithms, openpal::Timestamp session_start)
		{
			// TODO - add nonces and reset these also

			algorithms_ = algorithms;
			session_start_ = session_start;
		}

	private:

		
		SessionKeys keys_;
		Algorithms::Session algorithms_;
		openpal::Timestamp session_start_ = openpal::Timestamp::min_value();

    };

}

#endif
