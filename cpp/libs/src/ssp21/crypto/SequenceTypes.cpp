
#include "ssp21/crypto/SequenceTypes.h"

#include "openpal/logging/LogMacros.h"

#include "openpal/serialization/BigEndian.h"
#include "ssp21/crypto/MessageParser.h"

using namespace openpal;

namespace ssp21
{
	template <class CountType, class SeqType>
	ParseError read_seq(openpal::RSlice& input, SeqType& value)
	{
		typename CountType::type_t count;
		auto err = MessageParser::read_fields(input, count);
		if (any(err)) return err;

		if (input.length() < count)
		{
			return ParseError::insufficient_bytes;
		}

		value = SeqType(input.take(count));
		input.advance(count);
		return ParseError::ok;
	}

	ParseError Seq8::read(openpal::RSlice& input)
	{
		return read_seq<UInt8, Seq8>(input, *this);
	}

	void Seq8::print(const char* name, IMessagePrinter& printer) const
	{
		printer.print(name, *this);
	}

	ParseError Seq16::read(openpal::RSlice& input)
	{
		return read_seq<UInt16, Seq16>(input, *this);
	}

	void Seq16::print(const char* name, IMessagePrinter& printer) const
	{
		printer.print(name, *this);
	}

    SeqRSlice::SeqRSlice() : count_(0)
    {}

	ParseError SeqRSlice::read(openpal::RSlice& input)
	{
		this->clear();

		uint8_t count;

		auto cerr = MessageParser::read_fields(input, count);
		if (any(cerr)) return cerr;

		while (count > 0)
		{
			Seq16 slice;
			auto serr = slice.read(input);
			if (any(serr)) return serr;

			if (!this->push(slice))
			{
				return ParseError::impl_capacity_limit;
			}
			--count;
		}

		return ParseError::ok;
	}

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

	/*
	

	ParseError MessageParser::read(openpal::RSlice& input, Seq8& value)
	{
	return read_seq<UInt8, Seq8>(input, value);
	}

	ParseError MessageParser::read(openpal::RSlice& input, Seq16& value)
	{
	return read_seq<UInt16, Seq16>(input, value);
	}

	ParseError MessageParser::read(openpal::RSlice& input, Seq8Seq16& value)
	{
	
	}
	*/

}
