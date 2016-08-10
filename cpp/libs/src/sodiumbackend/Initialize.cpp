
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
				&SHA256,
				&HMAC_SHA256
			);

			return true;
		}

		
	}
}


