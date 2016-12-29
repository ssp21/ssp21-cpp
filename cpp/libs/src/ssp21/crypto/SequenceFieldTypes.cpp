
#include "ssp21/crypto/SequenceFieldTypes.h"

#include "openpal/logging/LogMacros.h"

#include "openpal/serialization/BigEndian.h"

#include "ssp21/crypto/IntegerField.h"
#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessageFormatter.h"

using namespace openpal;

namespace ssp21
{
	/*
    template <class CountType, class SeqType>
    ParseError read_seq(openpal::RSlice& input, SeqType& value)
    {
        IntegerField<CountType> count;
        auto err = count.read(input);
        if (any(err)) return err;

        if (input.length() < count)
        {
            return ParseError::insufficient_bytes;
        }

		value = input.take(count);
        input.advance(count);
        return ParseError::ok;
    }

    template <class CountType, class SeqType>
    FormatError write_seq(openpal::WSlice& dest, const SeqType& value)
    {
        if (value.length() > CountType::max_value)
        {
            return FormatError::bad_sequence_length;
        }

        IntegerField<CountType> count(static_cast<typename CountType::type_t>(value.length()));

        auto err = count.write(dest);
        if (any(err)) return err;

        if (dest.length() < value.length()) return FormatError::insufficient_space;

		dest.copy_from(value.widen<uint32_t>());

        return FormatError::ok;
    }

    ParseError Seq8Field::read(openpal::RSlice& input)
    {
        return read_seq<UInt8, Seq8>(input, this->value);
    }

    FormatError Seq8Field::write(openpal::WSlice& output) const
    {
        return write_seq<UInt8, Seq8>(output, this->value);
    }

    void Seq8Field::print(const char* name, IMessagePrinter& printer) const
    {
        printer.print(name, this->value.widen<uint32_t>());
    }

    ParseError Seq16Field::read(openpal::RSlice& input)
    {
        return read_seq<UInt16, Seq16>(input, this->value);
    }

    FormatError Seq16Field::write(openpal::WSlice& output) const
    {
        return write_seq<UInt16, Seq16>(output, this->value);
    }

    void Seq16Field::print(const char* name, IMessagePrinter& printer) const
    {
        printer.print(name, this->value.widen<uint32_t>());
    }
	*/

    Seq8Seq16Field::Seq8Seq16Field() : count_(0)
    {}

    ParseError Seq8Seq16Field::read(openpal::RSlice& input)
    {
        this->clear();

        IntegerField<UInt8> count;

        auto cerr = count.read(input);
        if (any(cerr)) return cerr;

        while (count > 0)
        {
            SeqField<openpal::UInt16> slice;
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

    FormatError Seq8Seq16Field::write(openpal::WSlice& output) const
    {
        if (this->count() > UInt8::max_value)
        {
            return FormatError::bad_sequence_length;
        }


        IntegerField<UInt8> count(static_cast<UInt8::type_t>(this->count()));

        auto err = count.write(output);
        if (any(err)) return err;

        for (UInt8::type_t i = 0; i < count_; ++i)
        {
            SeqField<openpal::UInt16> slice(slices_[i]);
            auto serr = slice.write(output);
            if (any(serr)) return serr;
        }

        return FormatError::ok;
    }

    void Seq8Seq16Field::print(const char* name, IMessagePrinter& printer) const
    {
        char message[max_log_entry_size];
        SAFE_STRING_FORMAT(message, max_log_entry_size, "%s (count = %u)", name, this->count());
        printer.print(message);

        for (uint32_t i = 0; i < count_; ++i)
        {
            SAFE_STRING_FORMAT(message, max_log_entry_size, "#%u", i + 1);
            printer.print(message, slices_[i].widen<uint32_t>());
        }

    }

    void Seq8Seq16Field::clear()
    {
        count_ = 0;
    }

    bool Seq8Seq16Field::push(const Seq16& slice)
    {
        if (count_ == consts::crypto::max_seq_of_seq)
        {
            return false;
        }

        slices_[count_++] = slice;
        return true;
    }

    bool Seq8Seq16Field::read(uint32_t i, Seq16& slice) const
    {
        if (i >= count_)
        {
            return false;
        }

        slice = slices_[i];
        return true;
    }

    uint8_t Seq8Seq16Field::count() const
    {
        return count_;
    }

}
