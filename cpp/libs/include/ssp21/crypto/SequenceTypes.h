
#ifndef SSP21_SEQUENCETYPES_H
#define SSP21_SEQUENCETYPES_H

#include "openpal/container/RSeq.h"

#include <cstdint>

namespace ssp21
{
	typedef openpal::RSeq<uint8_t> Seq8;
	typedef openpal::RSeq<uint16_t> Seq16;
}

#endif
