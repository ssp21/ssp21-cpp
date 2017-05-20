
#include "MockCryptoBackend.h"

#include "testlib/HexConversions.h"
#include "ssp21/crypto/gen/CryptoError.h"
#include "ssp21/crypto/Crypto.h"

#include <cstring>

namespace ssp21
{	
	CryptoFixture* CryptoFixture::instance = nullptr;

	bool Crypto::initialize_impl()
	{
		return true;
	}

    void Crypto::zero_memory_impl(const wseq32_t& data)
    {
        memset(data, 0, data.length());
    }

    bool Crypto::secure_equals_impl(const seq8_t& lhs, const seq8_t& rhs)
    {
		assert(CryptoFixture::instance);
		CryptoFixture::instance->actions.push_back(CryptoAction::secure_equals);

        const auto lhs_string = openpal::to_hex(lhs);
        const auto rhs_string = openpal::to_hex(rhs);

        return lhs_string == rhs_string;
    }

    void Crypto::hash_sha256_impl(std::initializer_list<seq32_t> data, SecureBuffer& output)
    {
		assert(CryptoFixture::instance);
		CryptoFixture::instance->actions.push_back(CryptoAction::hash_sha256);
        output.as_wseq().take(consts::crypto::sha256_hash_output_length).set_all_to(CryptoFixture::instance->fill_byte);
        output.set_type(BufferType::sha256);
    }

    void Crypto::hmac_sha256_impl(const seq8_t& key, std::initializer_list<seq32_t> data, SecureBuffer& output)
    {
		assert(CryptoFixture::instance);
		CryptoFixture::instance->actions.push_back(CryptoAction::hmac_sha256);

        output.as_wseq().take(consts::crypto::sha256_hash_output_length).set_all_to(CryptoFixture::instance->fill_byte);
        output.set_type(BufferType::sha256);
    }

    void Crypto::hkdf_sha256_impl(const seq8_t& chaining_key, std::initializer_list<seq32_t> input_key_material, SymmetricKey& key1, SymmetricKey& key2)
    {
		assert(CryptoFixture::instance);
		CryptoFixture::instance->actions.push_back(CryptoAction::hkdf_sha256);

        for (auto key :
                {
                    &key1, &key2
                })
        {
            key->as_wseq().take(consts::crypto::sha256_hash_output_length).set_all_to(CryptoFixture::instance->fill_byte);
            key->set_type(BufferType::symmetric_key);
        }
    }

    void Crypto::gen_keypair_x25519_impl(KeyPair& pair)
    {
		assert(CryptoFixture::instance);
		CryptoFixture::instance->actions.push_back(CryptoAction::gen_keypair_x25519);

        pair.private_key.as_wseq().take(consts::crypto::x25519_key_length).set_all_to(CryptoFixture::instance->fill_byte);
        pair.private_key.set_type(BufferType::x25519_key);

        pair.public_key.as_wseq().take(consts::crypto::x25519_key_length).set_all_to(CryptoFixture::instance->fill_byte);
        pair.public_key.set_type(BufferType::x25519_key);
    }

    void Crypto::dh_x25519_impl(const PrivateKey& priv_key, const seq8_t& pub_key, DHOutput& output, std::error_code& ec)
    {
		assert(CryptoFixture::instance);
		CryptoFixture::instance->actions.push_back(CryptoAction::dh_x25519);

        if (CryptoFixture::instance->fail_dh_x25519)
        {
            ec = CryptoError::dh_x25519_fail;
            return;
        }

        output.as_wseq().take(consts::crypto::x25519_key_length).set_all_to(CryptoFixture::instance->fill_byte);
        output.set_type(BufferType::x25519_key);
    }

	void Crypto::gen_keypair_ed25519_impl(KeyPair& pair)
	{
		assert(CryptoFixture::instance);
		CryptoFixture::instance->actions.push_back(CryptoAction::gen_keypair_ed25519);

		pair.private_key.as_wseq().take(consts::crypto::ed25519_private_key_length).set_all_to(CryptoFixture::instance->fill_byte);
		pair.private_key.set_type(BufferType::ed25519_private_key);

		pair.public_key.as_wseq().take(consts::crypto::ed25519_public_key_length).set_all_to(CryptoFixture::instance->fill_byte);
		pair.public_key.set_type(BufferType::ed25519_public_key);
	}

	void Crypto::sign_ed25519_impl(const seq32_t& input, const PrivateKey& key, DSAOutput& output, std::error_code& ec)
	{
		assert(CryptoFixture::instance);
		CryptoFixture::instance->actions.push_back(CryptoAction::gen_keypair_ed25519);

		output.as_wseq().take(consts::crypto::ed25519_signature_length).set_all_to(CryptoFixture::instance->fill_byte);
		output.set_type(BufferType::ed25519_signature);
	}

}


