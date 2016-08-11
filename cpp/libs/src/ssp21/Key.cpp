
#include "ssp21/Key.h"

#include <assert.h>

namespace ssp21
{		

Key::Key() : length_(0)
{}
	
openpal::RSlice Key::as_slice()
{
	return buffer_.ToRSlice().Take(length_);
}

openpal::WSlice Key::get_write_slice()
{
	return buffer_.GetWSlice();
}

void Key::set_length(uint32_t length)
{
	assert(length_ <= buffer_.Size());
	length_ = length;
}

		
}

