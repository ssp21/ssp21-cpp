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
// License TBD
//

#include "ssp21/gen/HandshakeError.h"

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
            return HandshakeError::unsupported_dh_mode;
        case(0x3):
            return HandshakeError::unsupported_hash_mode;
        case(0x4):
            return HandshakeError::unsupported_session_mode;
        case(0x5):
            return HandshakeError::unsupported_certificate_mode;
        case(0x6):
            return HandshakeError::bad_certificate_format;
        case(0x7):
            return HandshakeError::unsupported_certificate_feature;
        case(0x8):
            return HandshakeError::authentication_error;
        case(0xFF):
            return HandshakeError::internal;
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
        case(HandshakeError::unsupported_dh_mode):
            return "unsupported_dh_mode";
        case(HandshakeError::unsupported_hash_mode):
            return "unsupported_hash_mode";
        case(HandshakeError::unsupported_session_mode):
            return "unsupported_session_mode";
        case(HandshakeError::unsupported_certificate_mode):
            return "unsupported_certificate_mode";
        case(HandshakeError::bad_certificate_format):
            return "bad_certificate_format";
        case(HandshakeError::unsupported_certificate_feature):
            return "unsupported_certificate_feature";
        case(HandshakeError::authentication_error):
            return "authentication_error";
        case(HandshakeError::internal):
            return "internal";
        default:
            return "undefined";
    }
}

}
