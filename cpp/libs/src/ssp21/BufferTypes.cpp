
#include "ssp21/BufferTypes.h"

#include "ssp21/Crypto.h"

namespace ssp21
{		

BufferBase::BufferBase() : length_(0), buffer_type_(BufferType::empty)
{}
	
openpal::RSlice BufferBase::as_slice() const
{
	return buffer_.as_rslice().take(length_);
}

BufferType BufferBase::get_type() const
{
	return buffer_type_;
}

openpal::WSlice BufferBase::get_write_slice()
{
	return buffer_.as_wslice();
}

void BufferBase::set_type(BufferType buffer_type)
{
	buffer_type_ = buffer_type;
	length_ = get_buffer_length(buffer_type);
}

uint32_t BufferBase::get_buffer_length(BufferType key_type)
{
	switch (key_type)
	{
		case(BufferType::x25519_key) :
			return consts::x25519_key_length;
		case(BufferType::sha256) :
			return consts::sha256_hash_output_length;
		case(BufferType::symmetric_key) :
			return consts::symmetric_ley_length;
		default:
			return 0;
	}
}

SecureBuffer::~SecureBuffer()
{
	Crypto::zero_memory(buffer_.as_wslice());
}

}

