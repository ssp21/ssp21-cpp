
#include "ssp21/crypto/SequenceTypes.h"

namespace ssp21
{


    SeqRSlice::SeqRSlice() : count_(0)
    {}

    void SeqRSlice::clear()
    {
        count_ = 0;
    }

    bool SeqRSlice::push(const openpal::RSlice& slice)
    {
        if (count_ == consts::max_seq_of_seq)
        {
            return false;
        }

        slices_[count_++] = slice;
        return true;
    }

    bool SeqRSlice::read(uint32_t i, openpal::RSlice& slice) const
    {
        if (i >= count_)
        {
            return false;
        }

        slice = slices_[i];
        return true;
    }

    uint32_t SeqRSlice::count() const
    {
        return count_;
    }

}
