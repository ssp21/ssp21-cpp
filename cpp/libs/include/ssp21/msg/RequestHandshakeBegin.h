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

#include "ssp21/gen/ParseError.h"
#include "ssp21/gen/CertificateMode.h"
#include "ssp21/gen/DHMode.h"
#include "ssp21/gen/FormatError.h"
#include "ssp21/gen/HashMode.h"
#include "ssp21/SequenceTypes.h"
#include "ssp21/gen/SessionMode.h"
#include "ssp21/gen/NonceMode.h"
#include "openpal/container/WSlice.h"
#include "openpal/util/Uncopyable.h"
#include "openpal/container/RSlice.h"
#include <cstdint>

namespace ssp21 {

struct RequestHandshakeBegin : openpal::Uncopyable
{
  RequestHandshakeBegin();

  ParseError read(openpal::RSlice& input);

  FormatError write(openpal::WSlice& dest);

  uint16_t version;
  NonceMode nonce_mode;
  DHMode dh_mode;
  HashMode hash_mode;
  SessionMode session_mode;
  CertificateMode certificate_mode;
  Seq8 ephemeral_public_key;
  Seq8Seq16 certificates;
};

}

#endif
