#include "MockCertificateData.h"

#include "ssp21/crypto/Constants.h"
#include "ssp21/util/Exception.h"
#include "ssp21/util/SerializationUtils.h"

namespace ssp21 {

MockCertificateData::MockCertificateData(uint8_t signing_level, PublicKeyType public_key_type)
    : public_key_data(allocate(get_size(public_key_type)))
    , signature_data(allocate(consts::crypto::ed25519_signature_length))
    , body(
          0x00000000,
          0xFFFFFFFF,
          signing_level,
          public_key_type,
          public_key_data->as_rslice())
    , body_data(serialize::to_buffer(body))
    , envelope(
          signature_data->as_rslice(),
          body_data->as_rslice())
{
}

uint8_t MockCertificateData::get_size(PublicKeyType type)
{
    switch (type) {
    case (PublicKeyType::X25519):
        return consts::crypto::x25519_key_length;
    case (PublicKeyType::Ed25519):
        return consts::crypto::ed25519_public_key_length;
    default:
        throw Exception("unknown public key type");
    }
}

std::unique_ptr<ser4cpp::Buffer> MockCertificateData::allocate(uint8_t size)
{
    auto ret = std::make_unique<ser4cpp::Buffer>(size);
    ret->as_wslice().set_all_to(0xFF);
    return ret;
}

}
