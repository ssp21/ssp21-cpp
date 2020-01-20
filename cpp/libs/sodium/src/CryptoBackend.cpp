

#include "sodium/CryptoBackend.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/gen/CryptoError.h"

#include "HKDF.h"

#include <sodium.h>

#include <ser4cpp/serialization/BigEndian.h>

namespace ssp21 {
namespace sodium {

    // assertions for SHA-256 related constants
    static_assert(crypto_hash_sha256_BYTES == crypto_auth_hmacsha256_BYTES, "sha256 hash and HMAC length mismatch");
    static_assert(consts::crypto::sha256_hash_output_length == crypto_hash_sha256_BYTES, "sha256 length mismatch");
    static_assert(consts::crypto::sha256_hash_output_length == crypto_auth_hmacsha256_BYTES, "sha256-HMAC length mismatch");

    // assertions for DH key lengths
    static_assert(consts::crypto::x25519_key_length == crypto_scalarmult_BYTES, "X25519 key length mismatch");

    // assertions for DSA key/signature lengths
    static_assert(consts::crypto::ed25519_public_key_length == crypto_sign_PUBLICKEYBYTES, "ed25519 public key length mismatch");
    static_assert(consts::crypto::ed25519_private_key_length == crypto_sign_SECRETKEYBYTES, "ed25519 private key length mismatch");
    static_assert(consts::crypto::ed25519_signature_length == crypto_sign_BYTES, "ed25519 signature length mismatch");

    // assertions for AESGCM
    static_assert(crypto_aead_aes256gcm_KEYBYTES == consts::crypto::symmetric_key_length, "GCM key not the same size as SSP21 key");
    static_assert(crypto_aead_aes256gcm_ABYTES == consts::crypto::aes_gcm_tag_length, "GCM auth tag mismatch");
    static_assert(crypto_aead_aes256gcm_ABYTES <= consts::crypto::max_primitive_buffer_length, "GCM auth tag cannot fit inside the primitive buffer");
    static_assert(crypto_aead_aes256gcm_NSECBYTES == 0, "Unexpected NSECBYTES for GCM");
    static_assert(crypto_aead_aes256gcm_NPUBBYTES == consts::crypto::aes_gcm_nonce_length, "Unexpected NPUBBYTES for GCM");

    class GCMNonceBuffer {
        uint8_t nonce_buffer[crypto_aead_aes256gcm_NPUBBYTES] = { 0x00 };

    public:
        seq32_t set(uint16_t value)
        {
            // The leftmost bytes are zero padded, with the last 2 byte being the big endian representation of the uint16_t nonce
            auto dest = wseq32_t(nonce_buffer, crypto_aead_aes256gcm_NPUBBYTES).skip(crypto_aead_aes256gcm_NPUBBYTES - 2);
            ser4cpp::UInt16::write_to(dest, value);
            return seq32_t(nonce_buffer, crypto_aead_aes256gcm_NPUBBYTES);
        }
    };

    bool CryptoBackend::initialize()
    {
        if (sodium_init() != 0)
            return false;
        return Crypto::initialize(std::make_shared<CryptoBackend>());
    }

    void CryptoBackend::zero_memory_impl(const wseq32_t& data)
    {
        sodium_memzero(data, data.length());
    }

    void CryptoBackend::gen_random_impl(const wseq32_t& dest)
    {
        randombytes_buf(dest, dest.length());
    }

    bool CryptoBackend::secure_equals_impl(const seq32_t& lhs, const seq32_t& rhs)
    {
        if (lhs.length() != rhs.length()) {
            return false;
        }

        return sodium_memcmp(lhs, rhs, lhs.length()) == 0;
    }

    void CryptoBackend::hash_sha256_impl(const std::initializer_list<seq32_t>& data, SecureBuffer& output)
    {
        crypto_hash_sha256_state state;
        crypto_hash_sha256_init(&state);

        for (auto& item : data) {
            crypto_hash_sha256_update(&state, item, item.length());
        }

        crypto_hash_sha256_final(&state, output.as_wseq());

        output.set_length(BufferLength::length_32);
    }

    void CryptoBackend::hmac_sha256_impl(const seq32_t& key, const std::initializer_list<seq32_t>& data, SecureBuffer& output)
    {
        crypto_auth_hmacsha256_state state;
        crypto_auth_hmacsha256_init(&state, key, key.length());

        for (auto& item : data) {
            crypto_auth_hmacsha256_update(&state, item, item.length());
        }

        crypto_auth_hmacsha256_final(&state, output.as_wseq());

        output.set_length(BufferLength::length_32);
    }

    void CryptoBackend::hkdf_sha256_impl(const seq32_t& salt, const std::initializer_list<seq32_t>& input_key_material, SymmetricKey& key1, SymmetricKey& key2)
    {
        hkdf<Crypto::hmac_sha256>(salt, input_key_material, key1, key2);
    }

    void CryptoBackend::gen_keypair_x25519_impl(KeyPair& pair)
    {
        auto dest = pair.private_key.as_wseq();
        randombytes_buf(dest, crypto_scalarmult_BYTES);

        // can't fail despite error code - NACL ABI relic
        crypto_scalarmult_base(pair.public_key.as_wseq(), dest);

        pair.public_key.set_length(BufferLength::length_32);
        pair.private_key.set_length(BufferLength::length_32);
    }

    void CryptoBackend::dh_x25519_impl(const PrivateKey& priv_key, const seq32_t& pub_key, DHOutput& output, std::error_code& ec)
    {
        if (crypto_scalarmult(output.as_wseq(), priv_key.as_seq(), pub_key) != 0) {
            ec = ssp21::CryptoError::dh_x25519_fail;
            return;
        }

        output.set_length(BufferLength::length_32);
    }

    void CryptoBackend::gen_keypair_ed25519_impl(KeyPair& pair)
    {
        auto publicDest = pair.public_key.as_wseq();
        auto privateDest = pair.private_key.as_wseq();

        // can't fail despite error code - NACL ABI relic
        crypto_sign_keypair(publicDest, privateDest);

        pair.public_key.set_length(BufferLength::length_32);
        pair.private_key.set_length(BufferLength::length_64);
    }

    void CryptoBackend::sign_ed25519_impl(const seq32_t& input, const seq32_t& key, DSAOutput& output, std::error_code& ec)
    {
        // can't fail despite error code - NACL ABI relic
        crypto_sign_detached(output.as_wseq(), nullptr, input, input.length(), key);

        output.set_length(BufferLength::length_64);
    }

    bool CryptoBackend::verify_ed25519_impl(const seq32_t& message, const seq32_t& signature, const seq32_t& public_key)
    {
        return crypto_sign_verify_detached(signature, message, message.length(), public_key) == 0;
    }

    AEADResult CryptoBackend::aes256_gcm_encrypt_impl(const SymmetricKey& key, uint16_t nonce, seq32_t ad, seq32_t plaintext, wseq32_t encrypt_buffer, MACOutput& mac)
    {
        GCMNonceBuffer nb{};

        const auto result = crypto_aead_aes256gcm_encrypt_detached(
            encrypt_buffer,
            mac.as_wseq(),
            nullptr, // MAC length output
            plaintext,
            plaintext.length(),
            ad,
            ad.length(),
            nullptr, // nsec
            nb.set(nonce),
            key.as_seq());

        if (result) {
            return AEADResult::failure(CryptoError::aead_encrypt_fail);
        }

        mac.set_length(BufferLength::length_16);

        return AEADResult::success(
            encrypt_buffer.readonly().take(plaintext.length()),
            mac.as_seq());
    }

    seq32_t CryptoBackend::aes256_gcm_decrypt_impl(const SymmetricKey& key, uint16_t nonce, seq32_t ad, seq32_t ciphertext, seq32_t auth_tag, wseq32_t cleartext, std::error_code& ec)
    {
        GCMNonceBuffer nb{};

        const auto result = crypto_aead_aes256gcm_decrypt_detached(
            cleartext,
            nullptr, //nsec
            ciphertext,
            ciphertext.length(),
            auth_tag,
            ad,
            ad.length(),
            nb.set(nonce),
            key.as_seq());

        if (result) {
            ec = CryptoError::aead_decrypt_fail;
            return seq32_t::empty();
        }

        return cleartext.readonly().take(ciphertext.length());
    }

}
}
