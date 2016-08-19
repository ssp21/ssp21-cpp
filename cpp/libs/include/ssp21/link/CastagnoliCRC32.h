#ifndef SSP21_CASTAGNOLI_CRC32_H
#define SSP21_CASTAGNOLI_CRC32_H

#include <cstdint>

#include <openpal/container/RSlice.h>
#include <openpal/util/Uncopyable.h>

namespace ssp21 {
	
	/**
	* CRC calculator for polynomial:
	* 
	* P(x) = x31 + x30 + x29 + x28 + x26 + x23 + x21 + x19 + x18 + x15 + x14 + x13 + x12 + x11 + x9 + x8 + x4 + x1 + 1
	*
	* notations:	
	* 
	* 0xF4ACFB13 (MSB-first)
	* 0xFA567D89 (Koopman)
	*
	*/
	struct CastagnoliCRC32 : private openpal::StaticOnly
	{
		static uint32_t calc(const openpal::RSlice& data);

	private:

		static const uint32_t table[256];
	};	

}

#endif
