#ifndef SSP21_HEXMESSAGEBUILDERS_H
#define SSP21_HEXMESSAGEBUILDERS_H

#include <string>
#include <cstdint>

#include "ssp21/crypto/gen/NonceMode.h"
#include "ssp21/crypto/gen/DHMode.h"
#include "ssp21/crypto/gen/HashMode.h"
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
            HashMode hash_mode,
            SessionMode session_mode,
            CertificateMode certificate_mode,
            const std::string& hex_ephem_pub_key,
            std::initializer_list<std::string> certificates = {}
        );

		std::string request_handshake_auth(const std::string& mac);

        std::string reply_handshake_begin(
            const std::string& hex_ephem_pub_key,
            std::initializer_list<std::string> certificates = {}
        );

        std::string reply_handshake_error(HandshakeError err);
    }
}

#endif