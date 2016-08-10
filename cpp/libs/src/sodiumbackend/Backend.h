

#ifndef SODIUMBACKEND_BACKEND_H
#define SODIUMBACKEND_BACKEND_H

#include <openpal/container/RSlice.h>
#include <openpal/container/WSlice.h>

#include <initializer_list>

namespace ssp21
{	
	namespace sodium
	{
		openpal::RSlice SHA256(std::initializer_list<openpal::RSlice> data, openpal::WSlice& dest);

		openpal::RSlice HMAC_SHA256(const openpal::RSlice& key, std::initializer_list<openpal::RSlice> data, openpal::WSlice& dest);
	}
}

#endif