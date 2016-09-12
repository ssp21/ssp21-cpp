#ifndef SSP21_ENUMFIELD_H
#define SSP21_ENUMFIELD_H

#include <cstdint>

#include "ssp21/crypto/IMessageField.h"

#include "openpal/serialization/BigEndian.h"

namespace ssp21 {

template <typename Spec>
class EnumField final : public IMessageField
{
	typedef typename Spec::enum_type_t enum_t;

public:

	operator enum_t& () { return value;  }
	operator enum_t () const { return value; }

	EnumField()
	{}

	EnumField(enum_t value) : value(value)
	{}

	virtual ParseError read(openpal::RSlice& input) override
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
	
	virtual FormatError write(openpal::WSlice& output) const override
	{
		return openpal::BigEndian::write(output, Spec::to_type(value)) ? FormatError::ok : FormatError::insufficient_space;
	}

	virtual void print(const char* name, IMessagePrinter& printer) const override
	{
		printer.print(name, Spec::to_string(this->value));
	}

	enum_t value = enum_t::undefined;
	
};
	
	
}

#endif
