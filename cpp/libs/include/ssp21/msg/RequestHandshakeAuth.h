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

#ifndef SSP21_REQUESTHANDSHAKEAUTH_H
#define SSP21_REQUESTHANDSHAKEAUTH_H

#include "ssp21/gen/Function.h"
#include "ssp21/SequenceTypes.h"
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

struct RequestHandshakeAuth : openpal::Uncopyable
{
  RequestHandshakeAuth();

  Seq8 mac;
};

}

#endif
