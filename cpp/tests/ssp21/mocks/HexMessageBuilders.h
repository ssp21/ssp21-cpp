#ifndef SSP21_HEXMESSAGEBUILDERS_H
#define SSP21_HEXMESSAGEBUILDERS_H

#include <string>
#include <cstdint>

#include "ssp21/crypto/gen/NonceMode.h"
#include "ssp21/crypto/gen/DHMode.h"
#include "ssp21/crypto/gen/HandshakeHash.h"
#include "ssp21/crypto/gen/HandshakeKDF.h"
#include "ssp21/crypto/gen/SessionMode.h"
#include "ssp21/crypto/gen/CertificateMode.h"
#include "ssp21/crypto/gen/HandshakeError.h"
#include "ssp21/crypto/gen/Function.h"

namespace ssp21
{
    namespace hex
    {

        std::string repeat(uint8_t value, uint8_t count);

        std::string func_to_hex(Function func);

        std::string request_handshake_begin(
            uint16_t version,
            NonceMode nonce_mode,
            DHMode dh_mode,
            HandshakeHash handshake_hash,
            HandshakeKDF handshake_kdf,
            SessionMode session_mode,
            uint16_t max_nonce,
            uint32_t max_session_time,
            CertificateMode certificate_mode,
            const std::string& hex_ephem_pub_key
        );

        std::string reply_handshake_begin(
            const std::string& hex_ephem_pub_key
        );

        std::string reply_handshake_error(HandshakeError err);

        std::string session_data(uint16_t nonce, uint32_t valid_until, const std::string& user_data, const std::string& auth_tag);

        std::string link_frame(uint16_t src, uint16_t dest, const std::string& payload);
    }
}

#endif