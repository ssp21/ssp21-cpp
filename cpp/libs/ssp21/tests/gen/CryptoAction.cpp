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

#include "gen/CryptoAction.h"

namespace ssp21 {

const char* CryptoActionSpec::to_string(CryptoAction arg)
{
    switch (arg) {
    case (CryptoAction::secure_equals):
        return "secure_equals";
    case (CryptoAction::gen_random):
        return "gen_random";
    case (CryptoAction::hash_sha256):
        return "hash_sha256";
    case (CryptoAction::hmac_sha256):
        return "hmac_sha256";
    case (CryptoAction::hkdf_sha256):
        return "hkdf_sha256";
    case (CryptoAction::gen_keypair_x25519):
        return "gen_keypair_x25519";
    case (CryptoAction::dh_x25519):
        return "dh_x25519";
    case (CryptoAction::gen_keypair_ed25519):
        return "gen_keypair_ed25519";
    case (CryptoAction::sign_ed25519):
        return "sign_ed25519";
    case (CryptoAction::verify_ed25519):
        return "verify_ed25519";
    case (CryptoAction::aes_gcm_encrypt):
        return "aes_gcm_encrypt";
    case (CryptoAction::aes_gcm_decrypt):
        return "aes_gcm_decrypt";
    default:
        return "undefined";
    }
}

}
