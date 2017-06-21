
#include "MockCryptoBackend.h"

#include "testlib/HexConversions.h"
#include "ssp21/crypto/gen/CryptoError.h"
#include "ssp21/crypto/Crypto.h"

#include "CryptoFixture.h"

#include <cstring>
#include <assert.h>

namespace ssp21
{
    std::shared_ptr<MockCryptoBackend> MockCryptoBackend::instance(nullptr);
    CryptoFixture* MockCryptoBackend::fixture(nullptr);

    bool MockCryptoBackend::initialize()
    {
        assert(!instance);
        instance = std::make_shared<MockCryptoBackend>();
        return Crypto::initialize(instance);
    }

    void MockCryptoBackend::zero_memory(const wseq32_t& data)
    {
        memset(data, 0, data.length());
    }

    bool MockCryptoBackend::secure_equals(const seq32_t& lhs, const seq32_t& rhs)
    {
        assert(fixture);
        fixture->actions.push_back(CryptoAction::secure_equals);

        const auto lhs_string = openpal::to_hex(lhs);
        const auto rhs_string = openpal::to_hex(rhs);

        return lhs_string == rhs_string;
    }

    void MockCryptoBackend::hash_sha256(std::initializer_list<seq32_t> data, SecureBuffer& output)
    {
        assert(fixture);
        fixture->actions.push_back(CryptoAction::hash_sha256);
        output.as_wseq().take(consts::crypto::sha256_hash_output_length).set_all_to(fixture->fill_byte);
        output.set_type(BufferType::sha256);
    }

    void MockCryptoBackend::hmac_sha256(const seq32_t& key, std::initializer_list<seq32_t> data, SecureBuffer& output)
    {
        assert(fixture);
        fixture->actions.push_back(CryptoAction::hmac_sha256);

        output.as_wseq().take(consts::crypto::sha256_hash_output_length).set_all_to(fixture->fill_byte);
        output.set_type(BufferType::sha256);
    }

    void MockCryptoBackend::hkdf_sha256(const seq32_t& chaining_key, std::initializer_list<seq32_t> input_key_material, SymmetricKey& key1, SymmetricKey& key2)
    {
        assert(fixture);
        fixture->actions.push_back(CryptoAction::hkdf_sha256);

        for (auto key :
                {
                    &key1, &key2
                })
        {
            key->as_wseq().take(consts::crypto::sha256_hash_output_length).set_all_to(fixture->fill_byte);
            key->set_type(BufferType::symmetric_key);
        }
    }

    void MockCryptoBackend::gen_keypair_x25519(KeyPair& pair)
    {
        assert(fixture);
        fixture->actions.push_back(CryptoAction::gen_keypair_x25519);

        pair.private_key.as_wseq().take(consts::crypto::x25519_key_length).set_all_to(fixture->fill_byte);
        pair.private_key.set_type(BufferType::x25519_key);

        pair.public_key.as_wseq().take(consts::crypto::x25519_key_length).set_all_to(fixture->fill_byte);
        pair.public_key.set_type(BufferType::x25519_key);
    }

    void MockCryptoBackend::dh_x25519(const PrivateKey& priv_key, const seq32_t& pub_key, DHOutput& output, std::error_code& ec)
    {
        assert(fixture);
        fixture->actions.push_back(CryptoAction::dh_x25519);

        if (fixture->fail_dh_x25519)
        {
            ec = CryptoError::dh_x25519_fail;
            return;
        }

        output.as_wseq().take(consts::crypto::x25519_key_length).set_all_to(fixture->fill_byte);
        output.set_type(BufferType::x25519_key);
    }

    void MockCryptoBackend::gen_keypair_ed25519(KeyPair& pair)
    {
        assert(fixture);
        fixture->actions.push_back(CryptoAction::gen_keypair_ed25519);

        pair.private_key.as_wseq().take(consts::crypto::ed25519_private_key_length).set_all_to(fixture->fill_byte);
        pair.private_key.set_type(BufferType::ed25519_private_key);

        pair.public_key.as_wseq().take(consts::crypto::ed25519_public_key_length).set_all_to(fixture->fill_byte);
        pair.public_key.set_type(BufferType::ed25519_public_key);
    }

    void MockCryptoBackend::sign_ed25519(const seq32_t& input, const seq32_t& private_key, DSAOutput& output, std::error_code& ec)
    {
        assert(fixture);
        fixture->actions.push_back(CryptoAction::sign_ed25519);

        output.as_wseq().take(consts::crypto::ed25519_signature_length).set_all_to(fixture->fill_byte);
        output.set_type(BufferType::ed25519_signature);
    }

    bool MockCryptoBackend::verify_ed25519(const seq32_t& message, const seq32_t& signature, const seq32_t& public_key)
    {
        assert(fixture);
        fixture->actions.push_back(CryptoAction::verify_ed25519);
        return true;
    }
}


