
#ifndef SSP21_CRYPTOLAYERCONFIG_H
#define SSP21_CRYPTOLAYERCONFIG_H

#include <cstdint>

#include "ssp21/crypto/Constants.h"

namespace ssp21
{

    struct CryptoLayerConfig
    {
        /// The maximum size of a reassembled message
        uint16_t max_reassembly_size = consts::link::max_config_payload_size;
    };

    struct ResponderConfig
    {
        CryptoLayerConfig config;
    };

    struct InitiatorConfig
    {
        /// The cyrptographic modes that the initiator will request from the responder
        struct Suite
        {
            NonceMode nonce_mode = NonceMode::increment_last_rx;
            DHMode dh_mode = DHMode::x25519;
            HandshakeHash handshake_hash = HandshakeHash::sha256;
            HandshakeKDF handshake_kdf = HandshakeKDF::hkdf_sha256;
            HandshakeMAC handshake_mac = HandshakeMAC::hmac_sha256;
            SessionMode session_mode = SessionMode::hmac_sha256_16;
        };

        // TODO - Add a copy constructor that applies reasonable limits/relationships to some of these values
        struct Params
        {
            /// How long the initiator will will for responses
            openpal::TimeDuration response_timeout = consts::crypto::initiator::default_response_timeout;

            /// How long the initiator will wait before retrying a failed timeout
            openpal::TimeDuration retry_timeout = consts::crypto::initiator::default_retry_timeout;

            /// The maximum session time that the initiator will request in the handshake
            uint32_t max_session_time_ms = consts::crypto::initiator::default_max_session_time_ms;

            /// The initiator will begin renegotiating when the session time reaches this value
            uint32_t session_time_renegotiation_trigger_ms = consts::crypto::initiator::default_session_time_renegotiation_trigger_ms;

            /// The maximum tx or rx nonce value that the initiator will request in the handshake
            uint16_t max_nonce_value = consts::crypto::initiator::default_max_nonce;

            /// The initiator will begin renegotiating when either nonce value reaches this trigger level
            uint16_t nonce_renegotiation_trigger_value = consts::crypto::initiator::default_nonce_renegotiation_trigger;
        };

        Suite suite;
        Params params;
        CryptoLayerConfig config;
    };

}

#endif
