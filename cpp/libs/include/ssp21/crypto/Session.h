
#ifndef SSP21_SESSION_H
#define SSP21_SESSION_H

#include "openpal/util/Uncopyable.h"
#include "ssp21/crypto/BufferTypes.h" 

namespace ssp21
{
    class Session : private openpal::Uncopyable
    {

    public:

		SessionKeys& get_keys() { return keys; }

       
	private:

		SessionKeys keys;

    };

}

#endif
