#ifndef SSP21_HEXSEQUENCES_H
#define SSP21_HEXSEQUENCES_H

#include "testlib/Hex.h"

#include "ssp21/crypto/SequenceTypes.h"

#include <limits>

namespace ssp21
{
    template <class N, class R>
    struct HexSeq : private openpal::Hex
    {
        HexSeq(const std::string& hex) : openpal::Hex(hex)
        {
            if (this->buffer_.length() >= std::numeric_limits<N>::max())
            {
                throw std::invalid_argument(hex);
            }
        }

        inline operator R() const
        {
            return to_seq();
        }

        inline R to_seq() const
        {
            return this->buffer_.as_rslice().take(static_cast<N>(this->buffer_.length()));
        }
    };

    typedef HexSeq<uint8_t, seq8_t> HexSeq8;
    typedef HexSeq<uint16_t, seq16_t> HexSeq16;
}

#endif