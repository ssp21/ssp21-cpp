#include "MockCertificateData.h"

#include "ssp21/crypto/Constants.h"
#include "ssp21/util/Exception.h"
#include "ssp21/util/SerializationUtils.h"

namespace ssp21
{

MockCertificateData::MockCertificateData(uint8_t signing_level, PublicKeyType public_key_type) :
	public_key_data(allocate(public_key_type)),
	body(
		0x00000000,
		0xFFFFFFFF,
		signing_level,
		public_key_type,
		public_key_data->as_rslice()
	),
	body_data(serialize::to_buffer(body)),
	envelope(
		seq32_t::empty(),
		seq32_t::empty(), // signature
		body_data->as_rslice()
	)
{
	
}

std::unique_ptr<openpal::Buffer> MockCertificateData::allocate(PublicKeyType type)
{	
	switch (type)
	{
		case(PublicKeyType::X25519):
			return initialize(std::make_unique<openpal::Buffer>(consts::crypto::x25519_key_length));
		case(PublicKeyType::Ed25519):
			return initialize(std::make_unique<openpal::Buffer>(consts::crypto::ed25519_public_key_length));
		default:
			throw Exception("unknown public key type");
	}
}

std::unique_ptr<openpal::Buffer> MockCertificateData::initialize(std::unique_ptr<openpal::Buffer> buffer)
{
	buffer->as_wslice().set_all_to(0xFF);
	return buffer;
}
	
}

