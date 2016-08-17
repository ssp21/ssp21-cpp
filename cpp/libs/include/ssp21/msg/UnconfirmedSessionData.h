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

#ifndef SSP21_UNCONFIRMEDSESSIONDATA_H
#define SSP21_UNCONFIRMEDSESSIONDATA_H

#include "ssp21/gen/ParseError.h"
#include "ssp21/gen/FormatError.h"
#include "ssp21/SequenceTypes.h"
#include "ssp21/FormatResult.h"
#include "openpal/container/WSlice.h"
#include "openpal/util/Uncopyable.h"
#include "openpal/container/RSlice.h"
#include <cstdint>

namespace ssp21 {

struct UnconfirmedSessionData : openpal::Uncopyable
{
  UnconfirmedSessionData();

  UnconfirmedSessionData(
    uint32_t valid_until_ms,
    uint16_t nonce,
    const Seq16& payload
  );

  ParseError read(openpal::RSlice& input);
  FormatError write(openpal::WSlice& output);
  FormatResult write_msg(openpal::WSlice& output);

  static const uint32_t min_size_bytes = 9;

  uint32_t valid_until_ms;
  uint16_t nonce;
  Seq16 payload;
};

}

#endif