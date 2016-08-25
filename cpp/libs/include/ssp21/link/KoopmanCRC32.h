#ifndef SSP21_KOOPMAN_CRC32_H
#define SSP21_KOOPMAN_CRC32_H

#include <cstdint>

#include <openpal/container/RSlice.h>
#include <openpal/util/Uncopyable.h>

namespace ssp21
{

/**
* CRC calculator for polynomial:
*
* P(x) = x^32 + x^29 + x^28 + x^25 + x^22 + x^20 + x^19 + x^13 + x^12 + x^10 + x^7 + x^4 + x^3 + 1
*
* notations:
*
* 0x32583499 (MSB-first)
* 0x992C1A4C (Koopman)
*
*/
struct KoopmanCRC32 : private openpal::StaticOnly
{
    static uint32_t calc(const openpal::RSlice& data);

private:

    static const uint32_t table[256];
};

}

#endif
