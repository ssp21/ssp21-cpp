
#ifndef SSP21_SEQUENCETYPES_H
#define SSP21_SEQUENCETYPES_H

#include "openpal/container/WSeq.h"

namespace ssp21
{
    typedef openpal::RSeq<uint32_t> seq32_t;
    typedef openpal::WSeq<uint32_t> wseq32_t;

	// Interface that can return a list of read-only sequences
	class ISeqList
	{
		virtual std::initializer_list<seq32_t> as_list() const = 0;
	};
}

#endif
