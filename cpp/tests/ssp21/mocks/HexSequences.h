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

        operator R() const
        {
            return R::from(
                       this->buffer_.as_rslice(),
                       static_cast<N>(this->buffer_.length())
                   );
        }

        R to_seq() const
        {
            return R::from(
                       this->buffer_.as_rslice(),
                       static_cast<N>(this->buffer_.length())
                   );
        }
    };

    typedef HexSeq<uint8_t, Seq8> HexSeq8;
    typedef HexSeq<uint16_t, Seq16> HexSeq16;
}

#endif