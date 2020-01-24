
#ifndef SSP21_SESSIONMODE_H
#define SSP21_SESSIONMODE_H

#include "IFrameWriter.h"
#include "crypto/gen/SessionData.h"
#include "ssp21/crypto/CryptoTypedefs.h"

namespace ssp21 {

class SessionMode {

    aead_encrypt_func_t encrypt;
    aead_decrypt_func_t decrypt;

public:
    SessionMode(aead_encrypt_func_t encrypt, aead_decrypt_func_t decrypt);

    seq32_t read(const SymmetricKey& key, const SessionData& msg, wseq32_t dest, std::error_code& ec) const;

    SessionData write(const SymmetricKey& key, const AuthMetadata& metadata, seq32_t& user_data, wseq32_t encrypt_buffer, MACOutput& mac, std::error_code& ec) const;
};

}

#endif
