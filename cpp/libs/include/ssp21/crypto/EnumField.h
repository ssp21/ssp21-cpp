#ifndef SSP21_ENUMFIELD_H
#define SSP21_ENUMFIELD_H

#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/IMessagePrinter.h"
#include "openpal/container/RSlice.h"
#include "openpal/container/WSlice.h"

#include "openpal/serialization/BigEndian.h"

namespace ssp21 {

template <typename Spec>
class EnumField final
{
	typedef typename Spec::enum_type_t enum_t;

public:

	operator enum_t& () { return value;  }
	operator enum_t () const { return value; }

	EnumField()
	{}

	EnumField(enum_t value) : value(value)
	{}

	ParseError read(openpal::RSlice& input)
	{
		uint8_t raw_value;
		if (!openpal::BigEndian::read(input, raw_value)) return ParseError::insufficient_bytes;

		auto enum_value = Spec::from_type(raw_value);

		if (enum_value == Spec::enum_type_t::undefined)
		{
			return ParseError::undefined_enum;
		}

		this->value = enum_value;

		return ParseError::ok;
	}
	
	FormatError write(openpal::WSlice& output) const
	{
		return openpal::BigEndian::write(output, Spec::to_type(value)) ? FormatError::ok : FormatError::insufficient_space;
	}
	
	void print(const char* name, IMessagePrinter& printer) const
	{
		printer.print(name, Spec::to_string(this->value));
	}

	enum_t value = enum_t::undefined;
	
};
	
	
}

#endif
