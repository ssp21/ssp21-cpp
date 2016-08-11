
#include "sodiumbackend/Initialize.h"

#include "ssp21/Crypto.h"

#include <sodium.h>

#include "Backend.h"


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
				&zero_memory,
				&calc_hash_sha256,
				&calc_hmac_sha256,
				&gen_keypair_x25519,
				&dh_x25519
			);

			return true;
		}

		
	}
}


