
#ifndef SSP21_HANDSHAKE_ALGORITHMS_H
#define SSP21_HANDSHAKE_ALGORITHMS_H

#include "CryptoTypedefs.h"

namespace ssp21
{			
	class HandshakeAlgorithms 
	{

	public:
						
		HandshakeAlgorithms(
			dh_func_t dh,
			hkdf_func_t hkdf,
			hash_func_t hash,
			gen_keypair_func_t gen_keypair
			) :
			dh_(dh),
			hkdf_(hkdf),
			hash_(hash),
			gen_keypair_(gen_keypair)
		{}

		void dh(
			const PrivateKey& priv_key,
			const PublicKey& pub_key,
			DHOutput& output,
			std::error_code& ec)
		{
			dh_(priv_key, pub_key, output, ec);
		}

		void hkdf(
			const openpal::RSlice& chaining_key,
			std::initializer_list<openpal::RSlice> input_key_material,
			SymmetricKey& key1,
			SymmetricKey& key2
			)
		{
			hkdf_(chaining_key, input_key_material, key1, key2);
		}

		void hash(std::initializer_list<openpal::RSlice> data, SecureBuffer &output)
		{
			hash_(data, output);
		}

		void gen_keypair(KeyPair& pair)
		{
			gen_keypair_(pair);
		}
		
	private:

		HandshakeAlgorithms() = delete;

		dh_func_t dh_;
		hkdf_func_t hkdf_;
		hash_func_t hash_;
		gen_keypair_func_t gen_keypair_;		
	};
}

#endif
