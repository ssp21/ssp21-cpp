

#include "Backend.h"

#include <sodium.h>
#include <assert.h>

#include "ssp21\Crypto.h"

namespace ssp21
{
	namespace sodium
	{
		static_assert(crypto_hash_sha256_BYTES == crypto_auth_hmacsha256_BYTES, "SHA256 hash and HMAC length mismatch");
		static_assert(Crypto::SHA256_HASH_OUTPUT_LENGTH == crypto_hash_sha256_BYTES, "SHA256 length mismatch");
		static_assert(Crypto::SHA256_HASH_OUTPUT_LENGTH == crypto_auth_hmacsha256_BYTES, "SHA256-HMAC length mismatch");
		

		openpal::RSlice SHA256(std::initializer_list<openpal::RSlice> data, openpal::WSlice& dest)
		{
			assert(dest.Size() >= crypto_hash_sha256_BYTES);

			crypto_hash_sha256_state state;
			crypto_hash_sha256_init(&state);

			for (auto& item : data)
			{
				crypto_hash_sha256_update(&state, item, item.Size());
			}


			crypto_hash_sha256_final(&state, dest);

			return dest.ToRSlice().Take(crypto_hash_sha256_BYTES);
		}

		openpal::RSlice HMAC_SHA256(const openpal::RSlice& key, std::initializer_list<openpal::RSlice> data, openpal::WSlice& dest)
		{
			assert(dest.Size() >= crypto_auth_hmacsha256_BYTES);

			crypto_auth_hmacsha256_state state;
			crypto_auth_hmacsha256_init(&state, key, key.Size());

			for (auto& item : data)
			{
				crypto_auth_hmacsha256_update(&state, item, item.Size());
			}


			crypto_auth_hmacsha256_final(&state, dest);

			return dest.ToRSlice().Take(crypto_auth_hmacsha256_BYTES);
		}
	}
}
