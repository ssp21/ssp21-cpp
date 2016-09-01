
#include "ssp21/crypto/SequenceTypes.h"

#include "openpal/logging/LogMacros.h"

using namespace openpal;

namespace ssp21
{
	void Seq8::print(const char* name, IMessagePrinter& printer) const
	{
		printer.print(name, *this);
	}

	void Seq16::print(const char* name, IMessagePrinter& printer) const
	{
		printer.print(name, *this);
	}

    SeqRSlice::SeqRSlice() : count_(0)
    {}

	void SeqRSlice::print(const char* name, IMessagePrinter& printer) const
	{		
		char message[max_log_entry_size];
		SAFE_STRING_FORMAT(message, max_log_entry_size, "%s (count = %u)", name, this->count());
		printer.print(message);
		
		for (uint32_t i = 0; i < count_; ++i)
		{						
			SAFE_STRING_FORMAT(message, max_log_entry_size, "#%u", i + 1);			
			printer.print(message, slices_[i]);
		}
		
	}

    void SeqRSlice::clear()
    {
        count_ = 0;
    }

    bool SeqRSlice::push(const openpal::RSlice& slice)
    {
        if (count_ == consts::crypto::max_seq_of_seq)
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
