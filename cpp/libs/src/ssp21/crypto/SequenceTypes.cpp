
#include "ssp21/crypto/SequenceTypes.h"

#include "openpal/logging/LogMacros.h"

#include "openpal/serialization/BigEndian.h"

#include "ssp21/crypto/IntegerField.h"
#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessageFormatter.h"

using namespace openpal;

namespace ssp21
{
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

        value = SeqType(input.take(count));
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

        value.copy_to(dest);

        return FormatError::ok;
    }

    ParseError Seq8::read(openpal::RSlice& input)
    {
        return read_seq<UInt8, Seq8>(input, *this);
    }

    FormatError Seq8::write(openpal::WSlice& output) const
    {
        return write_seq<UInt8, Seq8>(output, *this);
    }

    void Seq8::print(const char* name, IMessagePrinter& printer) const
    {
        printer.print(name, *this);
    }

    ParseError Seq16::read(openpal::RSlice& input)
    {
        return read_seq<UInt16, Seq16>(input, *this);
    }

    FormatError Seq16::write(openpal::WSlice& output) const
    {
        return write_seq<UInt16, Seq16>(output, *this);
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

        IntegerField<UInt8> count;

        auto cerr = count.read(input);
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

    FormatError SeqRSlice::write(openpal::WSlice& output) const
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
            Seq16 slice(slices_[i]);
            auto serr = slice.write(output);
            if (any(serr)) return serr;
        }

        return FormatError::ok;
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

}
