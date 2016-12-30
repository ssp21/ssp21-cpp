
#include "MockCryptoBackend.h"

#include "testlib/HexConversions.h"

#include <cstring>

namespace ssp21
{

    MockCryptoBackend MockCryptoBackend::instance;

    void MockCryptoBackend::zero_memory(wseq32_t data)
    {
        memset(data, 0, data.length());
    }

    bool MockCryptoBackend::secure_equals(const seq8_t& lhs, const seq8_t& rhs)
    {
        actions.push_back(CryptoAction::secure_equals);

        const auto lhs_string = openpal::to_hex(lhs);
        const auto rhs_string = openpal::to_hex(rhs);

        return lhs_string == rhs_string;
    }

    void MockCryptoBackend::hash_sha256(std::initializer_list<seq32_t> data, SecureBuffer& output)
    {
        actions.push_back(CryptoAction::hash_sha256);
        output.get_write_slice().take(consts::crypto::sha256_hash_output_length).set_all_to(fill_byte);
        output.set_type(BufferType::sha256);
    }

    void MockCryptoBackend::hmac_sha256(const seq8_t& key, std::initializer_list<seq32_t> data, SecureBuffer& output)
    {
        actions.push_back(CryptoAction::hmac_sha256);
        output.get_write_slice().take(consts::crypto::sha256_hash_output_length).set_all_to(fill_byte);
        output.set_type(BufferType::sha256);
    }

    void MockCryptoBackend::gen_keypair_x25519(KeyPair& pair)
    {
        actions.push_back(CryptoAction::gen_keypair_x25519);

        pair.private_key.get_write_slice().take(consts::crypto::x25519_key_length).set_all_to(fill_byte);
        pair.private_key.set_type(BufferType::x25519_key);

        pair.public_key.get_write_slice().take(consts::crypto::x25519_key_length).set_all_to(fill_byte);
        pair.public_key.set_type(BufferType::x25519_key);
    }

    void MockCryptoBackend::dh_x25519(const PrivateKey& priv_key, const seq8_t& pub_key, DHOutput& output, std::error_code& ec)
    {
        actions.push_back(CryptoAction::dh_x25519);

        output.get_write_slice().take(consts::crypto::x25519_key_length).set_all_to(fill_byte);
        output.set_type(BufferType::x25519_key);
    }

}


