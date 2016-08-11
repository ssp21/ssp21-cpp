
#include "sodiumbackend/Initialize.h"

#include "ssp21/Crypto.h"
#include "Backend.h"

#include <sodium.h>

namespace ssp21
{
	namespace sodium
	{

		bool initialize()
		{
			if (sodium_init() == -1)
			{
				return false;
			}
			
			// initialize concrete functions
			Crypto::inititalize(
				CryptoBackend(
					&zero_memory,
					&secure_equals,
					&calc_hash_sha256,
					&calc_hmac_sha256,
					&gen_keypair_x25519,
					&dh_x25519
				)				
			);

			return true;
		}

		
	}
}


