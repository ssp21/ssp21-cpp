
#include "ssp21/crypto/ICryptoBackend.h"

#include <ssp21/crypto/gen/CryptoError.h>

namespace ssp21 {

void ICryptoBackend::zero_memory(const wseq32_t& data)
{
    this->zero_memory_impl(data);
}

void ICryptoBackend::gen_random(const wseq32_t& dest)
{
    this->gen_random_impl(dest);
}

bool ICryptoBackend::secure_equals(const seq32_t& lhs, const seq32_t& rhs)
{
    return this->secure_equals_impl(lhs, rhs);
}

void ICryptoBackend::hash_sha256(
    const std::initializer_list<seq32_t>& data,
    SecureBuffer& output)
{
    this->hash_sha256_impl(data, output);
}

void ICryptoBackend::hmac_sha256(
    const seq32_t& key,
    const std::initializer_list<seq32_t>& data,
    SecureBuffer& output)
{
    this->hmac_sha256_impl(key, data, output);
}

void ICryptoBackend::gen_keypair_x25519(KeyPair& pair)
{
    this->gen_keypair_x25519_impl(pair);
}

void ICryptoBackend::dh_x25519(
    const PrivateKey& priv_key,
    const seq32_t& pub_key,
    DHOutput& output,
    std::error_code& ec)
{
    if ((priv_key.get_type() != BufferType::x25519_key) || (pub_key.length() != consts::crypto::x25519_key_length)) {
        ec = CryptoError::bad_key_type;
        return;
    }

    this->dh_x25519_impl(priv_key, pub_key, output, ec);
}

void ICryptoBackend::hkdf_sha256(
    const seq32_t& salt,
    const std::initializer_list<seq32_t>& input_key_material,
    SymmetricKey& key1,
    SymmetricKey& key2)
{
    this->hkdf_sha256_impl(salt, input_key_material, key1, key2);
}

void ICryptoBackend::gen_keypair_ed25519(KeyPair& pair)
{
    this->gen_keypair_ed25519_impl(pair);
}

void ICryptoBackend::sign_ed25519(const seq32_t& input, const seq32_t& private_key, DSAOutput& output, std::error_code& ec)
{
    if (private_key.length() != consts::crypto::ed25519_private_key_length) {
        ec = CryptoError::bad_length;
        return;
    }
    this->sign_ed25519_impl(input, private_key, output, ec);
}

bool ICryptoBackend::verify_ed25519(const seq32_t& message, const seq32_t& signature, const seq32_t& public_key)
{
    if (public_key.length() != consts::crypto::ed25519_public_key_length) {
        return false;
    }

    if (signature.length() != consts::crypto::ed25519_signature_length) {
        return false;
    }

    return this->verify_ed25519_impl(message, signature, public_key);
}

AEADResult ICryptoBackend::aes256_gcm_encrypt(const SymmetricKey& key, uint16_t nonce, seq32_t ad, seq32_t plaintext, wseq32_t encrypt_buffer, MACOutput& mac)
{
    if (encrypt_buffer.length() < plaintext.length()) {
        return AEADResult::failure(CryptoError::bad_buffer_size);
    }

    return this->aes256_gcm_encrypt_impl(key, nonce, ad, plaintext, encrypt_buffer, mac);
}

}
