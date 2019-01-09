
#ifndef SSP21_SEQUENCETYPES_H
#define SSP21_SEQUENCETYPES_H

#include "ser4cpp/container/WSeq.h"

namespace ssp21
{
    typedef ser4cpp::RSeq<uint32_t> seq32_t;
    typedef ser4cpp::WSeq<uint32_t> wseq32_t;
}

#endif
