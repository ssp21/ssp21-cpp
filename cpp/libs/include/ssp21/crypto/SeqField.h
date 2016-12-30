
#ifndef SSP21_SEQFIELD_H
#define SSP21_SEQFIELD_H

#include "openpal/container/WSlice.h"
#include "openpal/serialization/BigEndian.h"

#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/gen/ParseError.h"

#include "ssp21/crypto/IMessagePrinter.h"
#include "ssp21/crypto/IntegerField.h"
#include "ssp21/crypto/SequenceTypes.h"

namespace ssp21
{
    template <class T>
    class SeqField final : public openpal::RSeq<typename T::type_t>
    {
    public:

        typedef openpal::RSeq<typename T::type_t> seq_t;

        SeqField() {}

        SeqField& operator=(const seq_t& other)
        {
            seq_t::operator=(other);
            return *this;
        }

        explicit SeqField(const seq_t& value) : seq_t(value)
        {}

        ParseError read(openpal::RSlice& input)
        {
            IntegerField<T> count;
            auto err = count.read(input);
            if (any(err)) return err;

            if (input.length() < count)
            {
                return ParseError::insufficient_bytes;
            }

            *this = input.take(count.value);
            input.advance(count);
            return ParseError::ok;
        }

        FormatError write(openpal::WSlice& dest) const
        {
            IntegerField<T> count(this->length());

            auto err = count.write(dest);
            if (any(err)) return err;

            if (dest.length() < this->length()) return FormatError::insufficient_space;

            dest.copy_from(*this);

            return FormatError::ok;
        }

        void print(const char* name, IMessagePrinter& printer) const
        {
            printer.print(name, *this);
        }

    };

}

#endif
