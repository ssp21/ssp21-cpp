
#include "ssp21/crypto/Crypto.h"

#include "ssp21/crypto/gen/CryptoError.h"

#include <assert.h>

namespace ssp21 {
std::shared_ptr<ICryptoBackend> Crypto::backend(nullptr);

bool Crypto::initialize(const std::shared_ptr<ICryptoBackend>& backendImpl)
{
    if (backend)
        return false;
    if (!backendImpl)
        return false;
    backend = backendImpl;
    return true;
}

void Crypto::zero_memory(const wseq32_t& data)
{
    assert(backend);
    backend->zero_memory(data);
}

void Crypto::gen_random(const wseq32_t& dest)
{
    assert(backend);
    backend->gen_random(dest);
}

bool Crypto::secure_equals(const seq32_t& lhs, const seq32_t& rhs)
{
    assert(backend);
    return backend->secure_equals(lhs, rhs);
}

void Crypto::hash_sha256(
    const std::initializer_list<seq32_t>& data,
    SecureBuffer& output)
{
    assert(backend);
    backend->hash_sha256(data, output);
}

void Crypto::hmac_sha256(
    const seq32_t& key,
    const std::initializer_list<seq32_t>& data,
    SecureBuffer& output)
{
    assert(backend);
    backend->hmac_sha256(key, data, output);
}

void Crypto::gen_keypair_x25519(KeyPair& pair)
{
    assert(backend);
    backend->gen_keypair_x25519(pair);
}

void Crypto::dh_x25519(const PrivateKey& priv_key, const seq32_t& pub_key, DHOutput& output, std::error_code& ec)
{
    assert(backend);

    if ((priv_key.get_type() != BufferType::x25519_key) || (pub_key.length() != consts::crypto::x25519_key_length)) {
        ec = CryptoError::bad_key_type;
        return;
    }

    backend->dh_x25519(priv_key, pub_key, output, ec);
}

void Crypto::hkdf_sha256(
    const seq32_t& salt,
    const std::initializer_list<seq32_t>& input_key_material,
    SymmetricKey& output1,
    SymmetricKey& output2)
{
    assert(backend);
    backend->hkdf_sha256(salt, input_key_material, output1, output2);
}

void Crypto::gen_keypair_ed25519(KeyPair& pair)
{
    assert(backend);
    backend->gen_keypair_ed25519(pair);
}

void Crypto::sign_ed25519(const seq32_t& input, const seq32_t& private_key, DSAOutput& output, std::error_code& ec)
{
    assert(backend);
    if (private_key.length() != consts::crypto::ed25519_private_key_length) {
        ec = CryptoError::bad_length;
        return;
    }

    backend->sign_ed25519(input, private_key, output, ec);
}

bool Crypto::verify_ed25519(const seq32_t& message, const seq32_t& signature, const seq32_t& public_key)
{
    assert(backend);
    if (public_key.length() != consts::crypto::ed25519_public_key_length) {
        return false;
    }

    if (signature.length() != consts::crypto::ed25519_signature_length) {
        return false;
    }

    return backend->verify_ed25519(message, signature, public_key);
}

}
