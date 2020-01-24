
#ifndef SSP21_SESSION_MODES_H
#define SSP21_SESSION_MODES_H

#include "MacAdapters.h"
#include "SessionMode.h"
#include "ssp21/crypto/Crypto.h"

namespace ssp21 {

class SessionModes : private ser4cpp::StaticOnly {

public:
    static SessionMode hmac_sha_256_trunc16()
    {
        return SessionMode(
            aead_mac_encrypt<Crypto::hmac_sha256, consts::crypto::trunc16>,
            aead_mac_decrypt<Crypto::hmac_sha256, consts::crypto::trunc16>);
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
