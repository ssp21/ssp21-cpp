
#ifndef SSP21_SEQUENCEFIELDTYPES_H
#define SSP21_SEQUENCEFIELDTYPES_H

#include "openpal/container/RSlice.h"
#include "openpal/container/WSlice.h"

#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/Constants.h"

#include "ssp21/crypto/SequenceTypes.h"
#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21
{
    class Seq8Field final
    {
    public:

		Seq8Field() {}

		explicit Seq8Field(const Seq8& value) : value(value)
		{}

        ParseError read(openpal::RSlice& input);
        FormatError write(openpal::WSlice& output) const;
		void print(const char* name, IMessagePrinter& printer) const;

		inline operator Seq8& ()
		{
			return value;
		}

		inline operator const Seq8& () const
		{
			return value;
		}

		inline uint8_t length() const
		{
			return value.length();
		}

		Seq8 value;
    };

	class Seq16Field final
	{
	public:

		Seq16Field() {}

		explicit Seq16Field(const Seq16& value) : value(value)
		{}

		ParseError read(openpal::RSlice& input);
		FormatError write(openpal::WSlice& output) const;
		void print(const char* name, IMessagePrinter& printer) const;

		inline operator Seq16& ()
		{
			return value;
		}

		inline operator const Seq16& () const
		{
			return value;
		}

		inline uint16_t length() const
		{
			return value.length();
		}

		Seq16 value;
	};

    class Seq8Seq16Field
    {

    public:

        Seq8Seq16Field();

        ParseError read(openpal::RSlice& input);
        FormatError write(openpal::WSlice& output) const;
        void print(const char* name, IMessagePrinter& printer) const;

        void clear();

        bool push(const Seq16& slice);

        bool read(uint32_t i, Seq16& slice) const;

        uint8_t count() const;

    private:

        uint8_t count_;
        Seq16 slices_[consts::crypto::max_seq_of_seq];
    };

}

#endif
