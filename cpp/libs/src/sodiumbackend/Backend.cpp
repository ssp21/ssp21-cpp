

#include "Backend.h"

#include <sodium.h>
#include <assert.h>

#include "ssp21\Crypto.h"

namespace ssp21
{
	namespace sodium
	{
		/// assertiosn for SHA-256 related constants
		static_assert(crypto_hash_sha256_BYTES == crypto_auth_hmacsha256_BYTES, "SHA256 hash and HMAC length mismatch");
		static_assert(consts::SHA256_HASH_OUTPUT_LENGTH == crypto_hash_sha256_BYTES, "SHA256 length mismatch");
		static_assert(consts::SHA256_HASH_OUTPUT_LENGTH == crypto_auth_hmacsha256_BYTES, "SHA256-HMAC length mismatch");

		
		static_assert(consts::X25519_KEY_LENGTH == crypto_scalarmult_BYTES, "X25519 key length mismatch");
		
		void Zero(openpal::WSlice& buff)
		{
			sodium_memzero(buff, buff.Size());
		}

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

		void GenKeyPair_X25519(KeyPair& pair)
		{
			randombytes_buf(pair.priv(), sizeof pair.priv.Size());
			crypto_scalarmult_base(pair.pub(), pair.priv());
		}

		openpal::RSlice DH_X25519(const openpal::RSlice& priv_key, const openpal::RSlice& pub_key, openpal::WSlice& dest, std::error_code& ec)
		{
			if (crypto_scalarmult(dest, priv_key, pub_key) != 0)
			{
				ec = std::error_code(1, std::generic_category()); // TODO - make actual error codes
				return openpal::RSlice::Empty();
			}

			return dest.ToRSlice().Take(crypto_scalarmult_BYTES);
		}
	}
}
