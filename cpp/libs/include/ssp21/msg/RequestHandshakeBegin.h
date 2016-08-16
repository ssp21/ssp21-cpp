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

#ifndef SSP21_REQUESTHANDSHAKEBEGIN_H
#define SSP21_REQUESTHANDSHAKEBEGIN_H

#include "openpal/util/Uncopyable.h"
#include "ssp21/gen/SessionMode.h"
#include "ssp21/SeqRSlice.h"
#include "ssp21/gen/CertificateMode.h"
#include "ssp21/gen/NonceMode.h"
#include "ssp21/gen/DHMode.h"
#include "ssp21/gen/HashMode.h"
#include <cstdint>
#include "openpal/container/RSlice.h"
#include "ssp21/gen/Function.h"

namespace ssp21 {

struct RequestHandshakeBegin : openpal::Uncopyable
{
  Function function;
  uint16_t version;
  NonceMode nonce_mode;
  DHMode dh_mode;
  HashMode hash_mode;
  SessionMode session_mode;
  CertificateMode certificate_mode;
  openpal::RSlice ephemeral_public_key;
  SeqRSlice certificates;
};

}

#endif
