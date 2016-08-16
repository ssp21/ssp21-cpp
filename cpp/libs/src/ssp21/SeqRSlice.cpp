
#include "ssp21/SeqRSlice.h"

namespace ssp21
{						
	

SeqRSlice::SeqRSlice()
{}

void SeqRSlice::clear()
{
	count_ = 0;
}

bool SeqRSlice::push(const openpal::RSlice& slice)
{
	if (count_ == consts::max_certificate_chain) {
		return false;
	}

	slices_[count_++] = slice;			
	return true;
}

bool SeqRSlice::read(uint32_t i, openpal::RSlice& slice) const 
{
	if (i >= count_) {
		return false;
	}

	slice = slices_[i];
	return true;
}

uint32_t SeqRSlice::count() const 
{
	return count_;
}

}
