//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Licensed under the terms of the BSDv3 license
//

#include "ssp21/crypto/gen/HandshakeError.h"

namespace ssp21 {

uint8_t HandshakeErrorSpec::to_type(HandshakeError arg)
{
    return static_cast<uint8_t>(arg);
}
HandshakeError HandshakeErrorSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return HandshakeError::bad_message_format;
        case(0x1):
            return HandshakeError::unsupported_version;
        case(0x2):
            return HandshakeError::unsupported_handshake_ephemeral;
        case(0x3):
            return HandshakeError::unsupported_handshake_hash;
        case(0x4):
            return HandshakeError::unsupported_handshake_kdf;
        case(0x6):
            return HandshakeError::unsupported_session_mode;
        case(0x7):
            return HandshakeError::unsupported_nonce_mode;
        case(0x8):
            return HandshakeError::unsupported_handshake_mode;
        case(0x9):
            return HandshakeError::bad_certificate_format;
        case(0xA):
            return HandshakeError::bad_certificate_chain;
        case(0xB):
            return HandshakeError::unsupported_certificate_feature;
        case(0xC):
            return HandshakeError::authentication_error;
        case(0xD):
            return HandshakeError::no_prior_handshake_begin;
        case(0xE):
            return HandshakeError::key_not_found;
        case(0xFF):
            return HandshakeError::internal;
        case(0xFD):
            return HandshakeError::none;
        default:
            return HandshakeError::undefined;
    }
}
const char* HandshakeErrorSpec::to_string(HandshakeError arg)
{
    switch(arg)
    {
        case(HandshakeError::bad_message_format):
            return "bad_message_format";
        case(HandshakeError::unsupported_version):
            return "unsupported_version";
        case(HandshakeError::unsupported_handshake_ephemeral):
            return "unsupported_handshake_ephemeral";
        case(HandshakeError::unsupported_handshake_hash):
            return "unsupported_handshake_hash";
        case(HandshakeError::unsupported_handshake_kdf):
            return "unsupported_handshake_kdf";
        case(HandshakeError::unsupported_session_mode):
            return "unsupported_session_mode";
        case(HandshakeError::unsupported_nonce_mode):
            return "unsupported_nonce_mode";
        case(HandshakeError::unsupported_handshake_mode):
            return "unsupported_handshake_mode";
        case(HandshakeError::bad_certificate_format):
            return "bad_certificate_format";
        case(HandshakeError::bad_certificate_chain):
            return "bad_certificate_chain";
        case(HandshakeError::unsupported_certificate_feature):
            return "unsupported_certificate_feature";
        case(HandshakeError::authentication_error):
            return "authentication_error";
        case(HandshakeError::no_prior_handshake_begin):
            return "no_prior_handshake_begin";
        case(HandshakeError::key_not_found):
            return "key_not_found";
        case(HandshakeError::internal):
            return "internal";
        case(HandshakeError::none):
            return "none";
        default:
            return "undefined";
    }
}

}
