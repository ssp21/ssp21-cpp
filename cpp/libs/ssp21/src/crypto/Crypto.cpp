
#include "ssp21/crypto/Crypto.h"

#include "ssp21/crypto/gen/CryptoError.h"

#include "ssp21/util/Exception.h"

#include <iostream>
#include <stdlib.h>

namespace ssp21 {

bool Crypto::initialized(false);
CryptoBackend Crypto::backend;

void Crypto::initialize(const CryptoBackend& impl)
{
    if (initialized) {
        throw Exception("backend already initialized");
    }

    if (!impl.zero_memory) {
        throw Exception("backend must support zeroing memory");
    }

    if (!impl.gen_random) {
        throw Exception("backend must support generating secure random bytes");
    }

    if (!impl.secure_equals) {
        throw Exception("backend must support secure comparison");
    }

    // assign the implementation
    backend = impl;
    initialized = true;
}

void Crypto::check_initialized()
{
    if (!initialized) {
        std::cerr << "Cryptographic function called before intializing the backend" << std::endl;
        exit(-1);
    }
}

void Crypto::check_supports_sha256()
{
    check_initialized();
    if (!supports_sha256()) {
        std::cerr << "backend does not support SHA-256 operation" << std::endl;
        exit(-1);
    }
}

void Crypto::check_supports_x25519()
{
    check_initialized();
    if (!supports_x25519()) {
        std::cerr << "backend does not support x25519 operation" << std::endl;
        exit(-1);
    }
}

void Crypto::check_supports_ed25519()
{
    check_initialized();
    if (!supports_ed25519()) {
        std::cerr << "backend does not support ed25519 operation" << std::endl;
        exit(-1);
    }
}

void Crypto::check_supports_aes256_gcm()
{
    check_initialized();
    if (!supports_aes256_gcm()) {
        std::cerr << "backend does not support AES-GCM operation" << std::endl;
        exit(-1);
    }
}

/// ------ mandatory functions may be called without support checks -------

void Crypto::zero_memory(const wseq32_t& data)
{
    check_initialized();
    Crypto::backend.zero_memory(data);
}

void Crypto::gen_random(const wseq32_t& dest)
{
    check_initialized();
    Crypto::backend.gen_random(dest);
}

bool Crypto::secure_equals(const seq32_t& lhs, const seq32_t& rhs)
{
    check_initialized();
    return Crypto::backend.secure_equals(lhs, rhs);
}

bool Crypto::supports_sha256()
{
    return backend.hash_sha256 && backend.hkdf_sha256 && backend.hmac_sha256;
}

bool Crypto::supports_x25519()
{
    return backend.gen_keypair_x25519 && backend.dh_x25519;
}

bool Crypto::supports_ed25519()
{
    return backend.gen_keypair_ed25519 && backend.sign_ed25519 && backend.verify_ed25519;
}

bool Crypto::supports_aes256_gcm()
{
    return backend.aes256_gcm_encrypt && backend.aes256_gcm_decrypt;
}

/// ------ optional functions require a support check -------

void Crypto::hash_sha256(
    const std::initializer_list<seq32_t>& data,
    SecureBuffer& output)
{
    check_supports_sha256();
    Crypto::backend.hash_sha256(data, output);
}

void Crypto::hmac_sha256(
    const seq32_t& key,
    const std::initializer_list<seq32_t>& data,
    SecureBuffer& output)
{
    check_supports_sha256();
    Crypto::backend.hmac_sha256(key, data, output);
}

void Crypto::gen_keypair_x25519(KeyPair& pair)
{
    check_supports_ed25519();
    Crypto::backend.gen_keypair_x25519(pair);
}

void Crypto::dh_x25519(const PrivateKey& priv_key, const seq32_t& pub_key, DHOutput& output, std::error_code& ec)
{
    check_supports_x25519();
    Crypto::backend.dh_x25519(priv_key, pub_key, output, ec);
}

void Crypto::hkdf_sha256(
    const seq32_t& salt,
    const std::initializer_list<seq32_t>& input_key_material,
    SymmetricKey& output1,
    SymmetricKey& output2)
{
    check_supports_sha256();
    Crypto::backend.hkdf_sha256(salt, input_key_material, output1, output2);
}

void Crypto::gen_keypair_ed25519(KeyPair& pair)
{
    check_supports_ed25519();
    Crypto::backend.gen_keypair_ed25519(pair);
}

void Crypto::sign_ed25519(const seq32_t& input, const seq32_t& private_key, DSAOutput& output, std::error_code& ec)
{
    check_supports_ed25519();
    Crypto::backend.sign_ed25519(input, private_key, output, ec);
}

bool Crypto::verify_ed25519(const seq32_t& message, const seq32_t& signature, const seq32_t& public_key)
{
    check_supports_ed25519();
    return Crypto::backend.verify_ed25519(message, signature, public_key);
}

AEADResult Crypto::aes256_gcm_encrypt(const SymmetricKey& key, uint16_t nonce, seq32_t ad, seq32_t plaintext, wseq32_t encrypt_buffer, MACOutput& mac)
{
    check_supports_aes256_gcm();
    return Crypto::backend.aes256_gcm_encrypt(key, nonce, ad, plaintext, encrypt_buffer, mac);
}

seq32_t Crypto::aes256_gcm_decrypt(const SymmetricKey& key, uint16_t nonce, seq32_t ad, seq32_t ciphertext, seq32_t auth_tag, wseq32_t plaintext, std::error_code& ec)
{
    check_supports_aes256_gcm();
    return Crypto::backend.aes256_gcm_decrypt(key, nonce, ad, ciphertext, auth_tag, plaintext, ec);
}

}
