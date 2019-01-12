#ifndef SSP21_ADDRESSES_H
#define SSP21_ADDRESSES_H

/** @file
 * @brief Structure @ref ssp21::Addresses
 */

#include <cstdint>

namespace ssp21
{

/**
 * @brief Link-layer pair of addresses.
 */
struct Addresses
{
    /**
     * @brief Constructor.
     * @param destination Destination address
     * @param source Source address
     */
    Addresses(uint16_t destination = 0, uint16_t source = 0) :
        destination(destination), source(source)
    {}

    /**
     * @brief Destination address.
     */
    uint16_t destination;

    /**
     * @brief Source address.
     */
    uint16_t source;
};

}

#endif
