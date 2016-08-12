

#include "SodiumBackend.h"

#include <sodium.h>
#include <assert.h>

namespace ssp21
{
	namespace sodium
	{
		/// assertions for SHA-256 related constants
		static_assert(crypto_hash_sha256_BYTES == crypto_auth_hmacsha256_BYTES, "SHA256 hash and HMAC length mismatch");
		static_assert(consts::sha256_hash_output_length == crypto_hash_sha256_BYTES, "SHA256 length mismatch");
		static_assert(consts::sha256_hash_output_length == crypto_auth_hmacsha256_BYTES, "SHA256-HMAC length mismatch");

		/// assertions for DH key lengths
		static_assert(consts::x25519_key_length == crypto_scalarmult_BYTES, "X25519 key length mismatch");
		
		SodiumBackend SodiumBackend::backend_;

		void SodiumBackend::zero_memory(openpal::WSlice data)
		{
			sodium_memzero(data, data.Size());
		}

		bool SodiumBackend::secure_equals(const openpal::RSlice& lhs, const openpal::RSlice& rhs)
		{
			if (lhs.Size() != rhs.Size()) return false;

			return sodium_memcmp(lhs, rhs, lhs.Size()) == 0;
		}

		void SodiumBackend::hash_sha256(std::initializer_list<openpal::RSlice> data, HashOutput& output)
		{			
			crypto_hash_sha256_state state;
			crypto_hash_sha256_init(&state);

			for (auto& item : data)
			{
				crypto_hash_sha256_update(&state, item, item.Size());
			}

			crypto_hash_sha256_final(&state, output.get_write_slice());

			output.set_type(BufferType::SHA256);
		}

		void SodiumBackend::hmac_sha256(const openpal::RSlice& key, std::initializer_list<openpal::RSlice> data, HashOutput& output)
		{			
			crypto_auth_hmacsha256_state state;
			crypto_auth_hmacsha256_init(&state, key, key.Size());

			for (auto& item : data)
			{
				crypto_auth_hmacsha256_update(&state, item, item.Size());
			}


			crypto_auth_hmacsha256_final(&state, output.get_write_slice());

			output.set_type(BufferType::SHA256);
		}

		void SodiumBackend::gen_keypair_x25519(KeyPair& pair)
		{
			auto dest = pair.private_key.get_write_slice();
			randombytes_buf(dest, crypto_scalarmult_BYTES);
			crypto_scalarmult_base(pair.public_key.get_write_slice(), dest);

			pair.public_key.set_type(BufferType::X25519_KEY);
			pair.private_key.set_type(BufferType::X25519_KEY);
		}

		void SodiumBackend::dh_x25519(const PrivateKey& priv_key, const PublicKey& pub_key, DHOutput& output, std::error_code& ec)
		{
			if (crypto_scalarmult(output.get_write_slice(), priv_key.as_slice(), pub_key.as_slice()) != 0)
			{
				ec = std::error_code(1, std::generic_category()); // TODO - make actual error codes
				return;
			}

			output.set_type(BufferType::X25519_KEY);
		}
	}
}
