
#ifndef SSP21_SEQBYTEFIELD_H
#define SSP21_SEQBYTEFIELD_H

#include "openpal/serialization/BigEndian.h"

#include "ssp21/util/SequenceTypes.h"

#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/gen/ParseError.h"

#include "ssp21/crypto/IMessagePrinter.h"
#include "ssp21/crypto/IntegerField.h"


namespace ssp21
{
    template <class CountType>
    class SeqByteField final : public openpal::RSeq<typename CountType::type_t>
    {
    public:

		size_t size() const
		{
			return CountType::size + this->length();
		}

        typedef openpal::RSeq<typename CountType::type_t> seq_t;

        SeqByteField() {}

        SeqByteField& operator=(const seq_t& other)
        {
            seq_t::operator=(other);
            return *this;
        }

        explicit SeqByteField(const seq_t& value) : seq_t(value)
        {}

        ParseError read(seq32_t& input)
        {
            IntegerField<CountType> count;
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

        FormatError write(wseq32_t& dest) const
        {
            IntegerField<CountType> count(this->length());

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
