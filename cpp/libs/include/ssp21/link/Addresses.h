
#ifndef SSP21_ADDRESSES_H
#define SSP21_ADDRESSES_H

#include <cstdint>

namespace ssp21
{
    struct Addresses
    {
        Addresses() : destination(0), source(0)
        {}

        Addresses(uint16_t destination, uint16_t source) :
            destination(destination), source(source)
        {}

        uint16_t destination;
        uint16_t source;
    };



}

#endif
