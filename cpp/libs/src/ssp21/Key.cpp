
#include "ssp21/Key.h"

#include <assert.h>

namespace ssp21
{		

Key::Key() : length_(0), key_type_(KeyType::EMPTY)
{}
	
openpal::RSlice Key::as_slice() const
{
	return buffer_.ToRSlice().Take(length_);
}

KeyType Key::get_key_type() const
{
	return key_type_;
}

openpal::WSlice Key::get_write_slice()
{
	return buffer_.GetWSlice();
}

void Key::set_key_type(KeyType key_type)
{
	key_type_ = key_type;
	length_ = get_key_length(key_type);
}

uint32_t Key::get_key_length(KeyType key_type)
{
	switch (key_type)
	{
		case(KeyType::X25519) :
			return consts::X25519_KEY_LENGTH;
		default:
			return 0;
	}
}
		
}

