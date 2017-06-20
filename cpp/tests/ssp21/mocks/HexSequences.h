#ifndef SSP21_HEXSEQUENCES_H
#define SSP21_HEXSEQUENCES_H

#include "testlib/Hex.h"

#include "ssp21/util/SequenceTypes.h"

namespace ssp21
{
    struct HexSeq : private openpal::Hex
    {
        HexSeq(const std::string& hex) : openpal::Hex(hex)
        {}

        inline operator seq32_t() const
        {
            return to_seq();
        }

        inline seq32_t to_seq() const
        {
            return this->buffer_.as_rslice();
        }
    };
}

#endif