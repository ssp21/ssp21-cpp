
#include "ssp21/HashOutput.h"

namespace ssp21
{			
	
HashOutput::HashOutput() : length_(0), hash_type_(HashOutputType::EMPTY)
{}

openpal::RSlice HashOutput::as_slice() const
{
	return buffer_.ToRSlice().Take(length_);
}
			
HashOutputType HashOutput::get_type() const
{
	return hash_type_;
}

openpal::WSlice HashOutput::get_write_slice()
{
	return buffer_.GetWSlice();
}
			
void HashOutput::set_type(HashOutputType type)
{
	length_ = get_length(type);
	hash_type_ = type;
}


uint32_t HashOutput::get_length(HashOutputType type)
{
	switch (type)
	{
	case(HashOutputType::SHA256) :
		return consts::sha256_hash_output_length;
	default:
		return 0;
	}
}
			
}

