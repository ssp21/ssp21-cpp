#ifndef SSP21_STATICBUFFER_H
#define SSP21_STATICBUFFER_H

#include "openpal/container/WSlice.h"
#include "openpal/container/RSlice.h"
#include "openpal/util/Comparisons.h"

#include <cstdint>

#include "Crypto.h"

namespace ssp21
{

template <uint32_t SIZE>
class SecureBuffer
{

public:

	SecureBuffer()
	{}

	~SecureBuffer()
	{		
		Crypto::Zero(GetWSlice());
	}

	openpal::RSlice ToRSlice() const
	{
		return RSlice(buffer, SIZE);
	}

	openpal::RSlice ToRSlice(uint32_t maxSize) const
	{
		return RSlice(buffer, openpal::Min(SIZE, maxSize));
	}

	openpal::WSlice GetWSlice()
	{
		return openpal::WSlice(buffer, SIZE);
	}

	openpal::WSlice GetWSlice(uint32_t maxSize)
	{
		return openpal::WSlice(buffer, openpal::Min(SIZE, maxSize));
	}

	const uint8_t* operator()() const
	{
		return buffer;
	}

	uint8_t* operator()()
	{
		return buffer;
	}

	uint32_t Size() const
	{
		return SIZE;
	}

private:
	uint8_t buffer[SIZE];
};

}

#endif
