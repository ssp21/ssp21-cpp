
#ifndef SSP21_SEQUENCETYPES_H
#define SSP21_SEQUENCETYPES_H

#include "openpal/container/WSeq.h"

namespace ssp21
{
    typedef openpal::RSeq<uint8_t> seq8_t;
    typedef openpal::RSeq<uint16_t> seq16_t;
    typedef openpal::RSeq<uint32_t> seq32_t;

    typedef openpal::WSeq<uint32_t> wseq32_t;
}

#endif
