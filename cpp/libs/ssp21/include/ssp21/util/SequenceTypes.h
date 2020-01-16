
#ifndef SSP21_SEQUENCETYPES_H
#define SSP21_SEQUENCETYPES_H

#include "ser4cpp/container/WSeq.h"

namespace ssp21 {
using seq32_t = ser4cpp::RSeq<uint32_t>;
using wseq32_t = ser4cpp::WSeq<uint32_t>;
}

#endif
