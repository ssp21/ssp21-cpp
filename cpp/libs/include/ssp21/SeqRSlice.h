
#ifndef SSP21_SEQRSLICE_H
#define SSP21_SEQRSLICE_H

#include <openpal/util/Uncopyable.h>

#include <openpal/container/RSlice.h>

#include "ssp21/Constants.h"

namespace ssp21
{						
	class SeqRSlice final : openpal::Uncopyable
	{

	public:

		SeqRSlice();

		void clear();

		bool push(const openpal::RSlice& slice);

		bool read(uint32_t i, openpal::RSlice& slice) const;

		uint32_t count() const;

	private:

		uint32_t count_;
		openpal::RSlice slices_[consts::max_certificate_chain];
	};
}

#endif
