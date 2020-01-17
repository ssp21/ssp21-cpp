#ifndef SSP21_CONSTANTS_H
#define SSP21_CONSTANTS_H

#include <cstdint>

#include "exe4cpp/Typedefs.h"
#include "ssp21/link/LinkConstants.h"

namespace ssp21 {
namespace consts {
    namespace crypto {
        const uint16_t protocol_version = 0x0000;

        // cryptographic constants
        const uint8_t sha256_hash_output_length = 32;
        const uint8_t x25519_key_length = 32;
        const uint8_t ed25519_public_key_length = 32;
        const uint8_t ed25519_private_key_length = 64;
        const uint8_t ed25519_signature_length = 64;
        const uint8_t aes_gcm_tag_length = 16;
        const uint8_t aes_gcm_nonce_length = 12;

        const uint8_t symmetric_key_length = 32;
        const uint8_t nonce_length = 32;

        // HMAC truncation lengths
        const uint8_t trunc16 = 16;

        // length of certificate issuer id
        const uint8_t issuer_id_length = 16;

        // maximum length_ required buffer_ length_ across algorithm types
        const uint8_t max_primitive_buffer_length = ed25519_private_key_length;

        // defaults for the Session
        const uint32_t default_ttl_pad_ms = 10000;

        // defaults for the initiator
        namespace initiator {
            const exe4cpp::duration_t default_response_timeout = std::chrono::seconds(2);
            const exe4cpp::duration_t default_retry_timeout = std::chrono::seconds(5);

            const uint16_t default_max_nonce = 32768;
            const uint16_t default_nonce_renegotiation_trigger = default_max_nonce - 128;

            const uint32_t default_max_session_time_ms = 1 * 60 * 60 * 1000;                                              // 1 hour in milliseconds
            const uint32_t default_session_time_renegotiation_trigger_ms = default_max_session_time_ms - (1 * 60 * 1000); // minus 1 minute in milliseconds
        }
    }
}
}

#endif
