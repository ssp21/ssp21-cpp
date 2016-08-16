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

#ifndef SSP21_REPLYHANDSHAKEBEGIN_H
#define SSP21_REPLYHANDSHAKEBEGIN_H

#include "openpal/util/Uncopyable.h"
#include "ssp21/gen/Function.h"
#include <cstdint>
#include "openpal/container/RSlice.h"
#include "ssp21/SeqRSlice.h"

namespace ssp21 {

struct ReplyHandshakeBegin : openpal::Uncopyable
{
  Function function;
  uint16_t version;
  openpal::RSlice ephemeral_public_key;
  SeqRSlice certificates;
};

}

#endif
