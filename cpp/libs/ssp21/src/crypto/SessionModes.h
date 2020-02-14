
#ifndef SSP21_SESSION_MODES_H
#define SSP21_SESSION_MODES_H

#include "MacAdapters.h"
#include "SessionMode.h"
#include "ssp21/crypto/Crypto.h"

namespace ssp21 {

class SessionModes : private ser4cpp::StaticOnly {

public:

    /*
     * This is required instead of just making the template
     * parameter the function itself b/c of bug/limitation
     * in GCC 5&6.
     */
    struct HMACSHA256 {
        static void calc(  const seq32_t& key,
                    const std::initializer_list<seq32_t>& data,
                    SecureBuffer& output)
        {
            Crypto::hmac_sha256(key, data, output);
        }
    };

    static SessionMode hmac_sha_256_trunc16()
    {
        return SessionMode(
            aead_mac_encrypt<HMACSHA256, consts::crypto::trunc16>,
            aead_mac_decrypt<HMACSHA256, consts::crypto::trunc16>);
    }

    static SessionMode aes_256_gcm()
    {
        return SessionMode(Crypto::aes256_gcm_encrypt, Crypto::aes256_gcm_decrypt);
    }

    static SessionMode default_mode()
    {
        return hmac_sha_256_trunc16();
    }
};

}

#endif
