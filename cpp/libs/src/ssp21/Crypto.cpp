
#include "ssp21/Crypto.h"

#include <assert.h>

namespace ssp21
{	
			
	ZeroT Crypto::m_zero_memory(nullptr);
	HASHFuncT Crypto::m_hash_sha256(nullptr);
	HMACFuncT Crypto::m_hmac_sha256(nullptr);
	GenKeyPairT Crypto::m_genpair_x25519(nullptr);
	DH_X25519T Crypto::m_dh_x25519(nullptr);


	void Crypto::Zero(openpal::WSlice& buff)
	{
		assert(m_zero_memory);
		m_zero_memory(buff);
	}

	openpal::RSlice Crypto::CalcHash_SHA256(
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest)
	{
		assert(m_hash_sha256);
		assert(dest.Size() >= consts::SHA256_HASH_OUTPUT_LENGTH);
		
		return m_hash_sha256(data, dest);
	}

	openpal::RSlice Crypto::CalcHMAC_SHA256(
		const openpal::RSlice& key,
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest)
	{
		assert(m_hmac_sha256);
		assert(dest.Size() >= consts::SHA256_HASH_OUTPUT_LENGTH);
		
		return m_hmac_sha256(key, data, dest);
	}

	void Crypto::GenKeyPair_X25519(KeyPair& pair)
	{
		assert(m_genpair_x25519);

		m_genpair_x25519(pair);
	}

	openpal::RSlice Crypto::DH_X25519(const openpal::RSlice& priv_key, const openpal::RSlice& pub_key, openpal::WSlice& dest, std::error_code& ec)
	{
		assert(m_dh_x25519);
		assert(pub_key.Size() == consts::X25519_KEY_LENGTH);
		assert(dest.Size() >= consts::X25519_KEY_LENGTH);
		
		return m_dh_x25519(priv_key, pub_key, dest, ec);
	}

	void Crypto::Inititalize(
		ZeroT zero_memory,
		HASHFuncT hash_sha256,
		HMACFuncT hmac_sha256,
		GenKeyPairT genpair_x25519,
		DH_X25519T dh_x25519)
	{
		m_zero_memory = zero_memory,
		m_hash_sha256 = hash_sha256;
		m_hmac_sha256 = hmac_sha256;
		m_genpair_x25519 = genpair_x25519;
		m_dh_x25519 = dh_x25519;
	}
}


