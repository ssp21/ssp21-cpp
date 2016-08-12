
#include "ssp21/BufferTypes.h"

#include "ssp21/Crypto.h"

#include <assert.h>

namespace ssp21
{		

Buffer::Buffer() : length_(0), buffer_type_(BufferType::EMPTY)
{}
	
openpal::RSlice Buffer::as_slice() const
{
	return buffer_.ToRSlice().Take(length_);
}

BufferType Buffer::get_type() const
{
	return buffer_type_;
}

openpal::WSlice Buffer::get_write_slice()
{
	return buffer_.GetWSlice();
}

void Buffer::set_type(BufferType buffer_type)
{
	buffer_type_ = buffer_type;
	length_ = get_buffer_length(buffer_type);
}

uint32_t Buffer::get_buffer_length(BufferType key_type)
{
	switch (key_type)
	{
		case(BufferType::X25519_KEY) :
			return consts::x25519_key_length;
		case(BufferType::SHA256) :
			return consts::sha256_hash_output_length;
		default:
			return 0;
	}
}

SecureBuffer::~SecureBuffer()
{
	Crypto::zero_memory(buffer_.GetWSlice());
}
		
}

