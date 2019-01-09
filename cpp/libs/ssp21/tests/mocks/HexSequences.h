#ifndef SSP21_HEXSEQUENCES_H
#define SSP21_HEXSEQUENCES_H

#include "ser4cpp/util/HexConversions.h"

#include "ssp21/util/SequenceTypes.h"

namespace ssp21
{
    struct HexSeq
    {
        HexSeq(const std::string& hex)
        {
            buffer_ = ser4cpp::HexConversions::from_hex(hex);
        }

        inline operator seq32_t() const
        {
            return to_seq();
        }

        inline seq32_t to_seq() const
        {
            return this->buffer_->as_rslice();
        }

        std::unique_ptr<ser4cpp::Buffer> buffer_;
    };
}

#endif