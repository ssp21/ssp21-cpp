
#ifndef SSP21_SEQUENCETYPES_H
#define SSP21_SEQUENCETYPES_H

#include <openpal/util/Uncopyable.h>

#include <openpal/container/RSlice.h>

#include "ssp21/crypto/Constants.h"

namespace ssp21
{				
	class Seq8 final : public openpal::RSlice
	{
	public:

		Seq8() : openpal::RSlice()
		{}

		explicit Seq8(const openpal::RSlice& other) : RSlice(other)
		{}
	};
	
	class Seq16 final : public openpal::RSlice 
	{
	public:
		Seq16() : openpal::RSlice()
		{}

		explicit Seq16(const openpal::RSlice& other) : RSlice(other)
		{}
	};

	class SeqRSlice
	{

	public:

		SeqRSlice();

		void clear();

		bool push(const openpal::RSlice& slice);

		bool read(uint32_t i, openpal::RSlice& slice) const;

		uint32_t count() const;

	private:

		uint32_t count_;
		openpal::RSlice slices_[consts::max_seq_of_seq];
	};

	// specialized types with type dependent constraints
	class Seq8Seq16 final : public SeqRSlice { };
}

#endif
