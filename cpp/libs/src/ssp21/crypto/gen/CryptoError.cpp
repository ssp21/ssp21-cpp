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

#include "ssp21/crypto/gen/CryptoError.h"

namespace ssp21 {

const char* CryptoErrorSpec::name = "crypto error";

const char* CryptoErrorSpec::to_string(CryptoError arg)
{
    switch(arg)
    {
        case(CryptoError::bad_key_type):
            return "bad_key_type";
        case(CryptoError::dh_x25519_fail):
            return "dh_x25519_fail";
        case(CryptoError::undefined_algorithm):
            return "undefined_algorithm";
        case(CryptoError::mac_auth_fail):
            return "mac_auth_fail";
        case(CryptoError::bad_buffer_size):
            return "bad_buffer_size";
        case(CryptoError::no_valid_session):
            return "no_valid_session";
        case(CryptoError::expired_ttl):
            return "expired_ttl";
        case(CryptoError::nonce_replay):
            return "nonce_replay";
        case(CryptoError::nonce_config_max):
            return "nonce_config_max";
        case(CryptoError::ttl_overflow):
            return "ttl_overflow";
        case(CryptoError::empty_user_data):
            return "empty_user_data";
        default:
            return "undefined";
    }
}

}
