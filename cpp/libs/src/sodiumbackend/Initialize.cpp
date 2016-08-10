
#include "sodiumbackend/Initialize.h"

#include "ssp21/Crypto.h"

#include <sodium.h>

#include "Backend.h"


namespace ssp21
{
	namespace sodium
	{

		bool Initialize()
		{
			if (sodium_init() == -1)
			{
				return false;
			}

			// initialize concrete functions
			Crypto::Inititalize(
				&Zero,
				&SHA256,
				&HMAC_SHA256,
				&GenKeyPair_X25519,
				&DH_X25519
			);

			return true;
		}

		
	}
}


