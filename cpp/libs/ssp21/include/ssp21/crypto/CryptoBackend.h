
#ifndef SSP21_CRYTPTOBACKEND_H
#define SSP21_CRYTPTOBACKEND_H

#include "ssp21/crypto/CryptoTypedefs.h"

namespace ssp21 {

// collection of mandatory and optional function pointers

struct CryptoBackend {

    CryptoBackend() = default;

    CryptoBackend(
        zero_memory_func_t zero_memory,
        gen_random_func_t gen_random,
        secure_equals_func_t secure_equals,
        hash_func_t hash_sha256,
        mac_func_t hmac_sha256,
        gen_keypair_func_t gen_keypair_x25519,
        dh_func_t dh_x25519,
        kdf_func_t hkdf_sha256,
        gen_keypair_func_t gen_keypair_ed25519,
        sign_dsa_func_t sign_ed25519,
        verify_dsa_func_t verify_ed25519,
        aead_encrypt_func_t aes256_gcm_encrypt,
        aead_decrypt_func_t aes256_gcm_decrypt)
        : zero_memory(zero_memory)
        , gen_random(gen_random)
        , secure_equals(secure_equals)
        , hash_sha256(hash_sha256)
        , hmac_sha256(hmac_sha256)
        , gen_keypair_x25519(gen_keypair_x25519)
        , dh_x25519(dh_x25519)
        , hkdf_sha256(hkdf_sha256)
        , gen_keypair_ed25519(gen_keypair_ed25519)
        , sign_ed25519(sign_ed25519)
        , verify_ed25519(verify_ed25519)
        , aes256_gcm_encrypt(aes256_gcm_encrypt)
        , aes256_gcm_decrypt(aes256_gcm_decrypt)
    {
    }

    ///  ---- MANDATORY primitives ------------

    /**
	*    If any of these are absent, it will cause
	*    an error during backend initialization.
	*/

    zero_memory_func_t zero_memory = nullptr;
    gen_random_func_t gen_random = nullptr;
    secure_equals_func_t secure_equals = nullptr;

    ///  ---- OPTIONAL primitives ------------

    /**
	*    If any of these are absent, and the initiator specifies
	*    one of them in configuration, it will causes an error
	*    during initialization.
	*
	*    If the initiator asks a responder for an unsupported
	*    algorithm it will respond with the appropriate handshake
	*    error message
	*/

    hash_func_t hash_sha256 = nullptr;
    mac_func_t hmac_sha256 = nullptr;
    gen_keypair_func_t gen_keypair_x25519 = nullptr;
    dh_func_t dh_x25519 = nullptr;
    kdf_func_t hkdf_sha256 = nullptr;
    gen_keypair_func_t gen_keypair_ed25519 = nullptr;
    sign_dsa_func_t sign_ed25519 = nullptr;
    verify_dsa_func_t verify_ed25519 = nullptr;
    aead_encrypt_func_t aes256_gcm_encrypt = nullptr;
    aead_decrypt_func_t aes256_gcm_decrypt = nullptr;
};

}

#endif
