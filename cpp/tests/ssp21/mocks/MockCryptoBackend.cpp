
#include "MockCryptoBackend.h"

#include <cstring>

namespace ssp21
{

MockCryptoBackend MockCryptoBackend::instance;

void MockCryptoBackend::zero_memory(openpal::WSlice data)
{	
	++counters.num_zero_memory;
	memset(data, 0, data.length());
}

bool MockCryptoBackend::secure_equals(const openpal::RSlice& lhs, const openpal::RSlice& rhs)
{
	++counters.num_secure_equals;
	return lhs.equals(rhs);
}

void MockCryptoBackend::hash_sha256(std::initializer_list<openpal::RSlice> data, SecureBuffer& output)
{
	++counters.num_hash_sha256;

	output.get_write_slice().take(consts::sha256_hash_output_length).set_all_to(fill_byte);
	output.set_type(BufferType::sha256);
}

void MockCryptoBackend::hmac_sha256(const openpal::RSlice& key, std::initializer_list<openpal::RSlice> data, SecureBuffer& output)
{
	++counters.num_hmac_sha256;

	output.get_write_slice().take(consts::sha256_hash_output_length).set_all_to(fill_byte);
	output.set_type(BufferType::sha256);
}

void MockCryptoBackend::gen_keypair_x25519(KeyPair& pair)
{
	++counters.num_gen_keypair_x25519;

	pair.private_key.get_write_slice().take(consts::x25519_key_length).set_all_to(fill_byte);
	pair.private_key.set_type(BufferType::x25519_key);

	pair.public_key.get_write_slice().take(consts::x25519_key_length).set_all_to(fill_byte);
	pair.public_key.set_type(BufferType::x25519_key);
}
		
void MockCryptoBackend::dh_x25519(const PrivateKey& priv_key, const openpal::RSlice& pub_key, DHOutput& output, std::error_code& ec)
{
	++counters.num_dh_x25519;

	output.get_write_slice().take(consts::x25519_key_length).set_all_to(fill_byte);
	output.set_type(BufferType::x25519_key);
}

}


