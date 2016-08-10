
#include "ssp21/Crypto.h"

#include <assert.h>

namespace ssp21
{	
	HASHFuncT Crypto::m_hash_sha256(nullptr);
	HMACFuncT Crypto::m_hmac_sha256(nullptr);

	openpal::RSlice Crypto::CalcHash_SHA256(
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest)
	{
		assert(dest.Size() >= SHA256_HASH_OUTPUT_LENGTH);
		assert(m_hash_sha256);

		return m_hash_sha256(data, dest);
	}

	openpal::RSlice Crypto::CalcHMAC_SHA256(
		const openpal::RSlice& key,
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest)
	{
		assert(dest.Size() >= SHA256_HASH_OUTPUT_LENGTH);
		assert(m_hmac_sha256);

		return m_hmac_sha256(key, data, dest);
	}


	void Crypto::Inititalize(
		HASHFuncT hash_sha256,
		HMACFuncT hmac_sha256)
	{
		m_hash_sha256 = hash_sha256;
		m_hmac_sha256 = hmac_sha256;
	}
}


