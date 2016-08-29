
#include "MockCryptoBackend.h"

namespace ssp21
{

MockCryptoBackend MockCryptoBackend::instance;

void MockCryptoBackend::zero_memory(openpal::WSlice data)
{

}

bool MockCryptoBackend::secure_equals(const openpal::RSlice& lhs, const openpal::RSlice& rhs)
{
	return lhs.equals(rhs);
}

void MockCryptoBackend::hash_sha256(std::initializer_list<openpal::RSlice> data, SecureBuffer& output)
{

}

void MockCryptoBackend::hmac_sha256(const openpal::RSlice& key, std::initializer_list<openpal::RSlice> data, SecureBuffer& output)
{

}

void MockCryptoBackend::gen_keypair_x25519(KeyPair& pair)
{

}
		
void MockCryptoBackend::dh_x25519(const PrivateKey& priv_key, const openpal::RSlice& pub_key, DHOutput& output, std::error_code& ec)
{

}

}


