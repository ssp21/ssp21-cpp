
#ifndef SSP21_ADDRESSES_H
#define SSP21_ADDRESSES_H

#include <cstdint>

namespace ssp21
{
    /**
    * Addressing information from a source to a destination
    */
    struct Addresses
    {
        Addresses(uint16_t destination, uint16_t source) :
            destination(destination), source(source)
        {}

        Addresses() : destination(0), source(0)
        {}

        uint16_t destination;
        uint16_t source;
    };

}

#endif