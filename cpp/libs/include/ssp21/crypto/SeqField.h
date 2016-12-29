
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
    class SeqField final
    {
    public:

        typedef openpal::RSeq<typename T::type_t> seq_t;

        SeqField() {}

        SeqField& operator=(const seq_t& other)
        {
            this->seq = other;
            return *this;
        }

        explicit SeqField(const seq_t& value) : seq(value)
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

            seq = input.take(count.value);
            input.advance(count);
            return ParseError::ok;
        }

        FormatError write(openpal::WSlice& dest) const
        {
            IntegerField<T> count(seq.length());

            auto err = count.write(dest);
            if (any(err)) return err;

            if (dest.length() < seq.length()) return FormatError::insufficient_space;

            dest.copy_from(seq.widen());

            return FormatError::ok;
        }

        void print(const char* name, IMessagePrinter& printer) const
        {
            printer.print(name, seq.widen());
        }

        inline operator seq_t& ()
        {
            return seq;
        }

        inline operator const seq_t& () const
        {
            return seq;
        }

        inline typename T::type_t length() const
        {
            return seq.length();
        }

        seq_t seq;
    };

}

#endif
