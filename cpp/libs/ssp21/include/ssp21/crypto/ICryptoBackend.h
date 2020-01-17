
#ifndef SSP21_ICRYTPTOBACKEND_H
#define SSP21_ICRYTPTOBACKEND_H

#include "ssp21/crypto/BufferTypes.h"

#include "ser4cpp/util/Uncopyable.h"

#include <initializer_list>
#include <system_error>

namespace ssp21 {

struct AEADResult {
    std::error_code ec;
    seq32_t ciphertext;
    seq32_t auth_tag;

    static AEADResult success(seq32_t ciphertext, seq32_t auth_tag)
    {
        return AEADResult{ std::error_code(), ciphertext, auth_tag };
    }

    static AEADResult failure(std::error_code ec)
    {
        return AEADResult{ ec, seq32_t::empty(), seq32_t::empty() };
    }
};

/**
* Pluggable crypto backend
* 
* Uses NVII idiom to enforce argument checks consistently across all implementations.
* Also means that implemenations don't have to do repetitive argument checking.
*
*/
class ICryptoBackend : private ser4cpp::Uncopyable {

public:
    virtual ~ICryptoBackend() {}

    void zero_memory(const wseq32_t& data);

    void gen_random(const wseq32_t& dest);

    bool secure_equals(const seq32_t& lhs, const seq32_t& rhs);

    void hash_sha256(
        const std::initializer_list<seq32_t>& data,
        SecureBuffer& output);

    void hmac_sha256(
        const seq32_t& key,
        const std::initializer_list<seq32_t>& data,
        SecureBuffer& output);

    void gen_keypair_x25519(KeyPair& pair);

    void dh_x25519(
        const PrivateKey& priv_key,
        const seq32_t& pub_key,
        DHOutput& output,
        std::error_code& ec);

    void hkdf_sha256(
        const seq32_t& salt,
        const std::initializer_list<seq32_t>& input_key_material,
        SymmetricKey& key1,
        SymmetricKey& key2);

    void gen_keypair_ed25519(KeyPair& pair);

    void sign_ed25519(const seq32_t& input, const seq32_t& private_key, DSAOutput& output, std::error_code& ec);

    bool verify_ed25519(const seq32_t& message, const seq32_t& signature, const seq32_t& public_key);

    AEADResult aes256_gcm_encrypt(const SymmetricKey& key, uint16_t nonce, seq32_t ad, seq32_t plaintext, wseq32_t encrypt_buffer, MACOutput& mac);

protected:
    virtual void zero_memory_impl(const wseq32_t& data) = 0;

    virtual void gen_random_impl(const wseq32_t& dest) = 0;

    virtual bool secure_equals_impl(const seq32_t& lhs, const seq32_t& rhs) = 0;

    virtual void hash_sha256_impl(
        const std::initializer_list<seq32_t>& data,
        SecureBuffer& output)
        = 0;

    virtual void hmac_sha256_impl(
        const seq32_t& key,
        const std::initializer_list<seq32_t>& data,
        SecureBuffer& output)
        = 0;

    virtual void gen_keypair_x25519_impl(KeyPair& pair) = 0;

    virtual void dh_x25519_impl(
        const PrivateKey& priv_key,
        const seq32_t& pub_key,
        DHOutput& output,
        std::error_code& ec)
        = 0;

    virtual void hkdf_sha256_impl(
        const seq32_t& salt,
        const std::initializer_list<seq32_t>& input_key_material,
        SymmetricKey& key1,
        SymmetricKey& key2)
        = 0;

    virtual void gen_keypair_ed25519_impl(KeyPair& pair) = 0;

    virtual void sign_ed25519_impl(const seq32_t& input, const seq32_t& private_key, DSAOutput& output, std::error_code& ec) = 0;

    virtual bool verify_ed25519_impl(const seq32_t& message, const seq32_t& signature, const seq32_t& public_key) = 0;

    virtual AEADResult aes256_gcm_encrypt_impl(const SymmetricKey& key, uint16_t nonce, seq32_t ad, seq32_t plaintext, wseq32_t encrypt_buffer, MACOutput& mac) = 0;
};
}

#endif
