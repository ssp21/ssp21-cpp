
#include "ssp21/MessageParser.h"

#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace ssp21 {

	template <class T>
	ParseResult read_integer(RSlice& input, typename T::type_t& value)
	{
		if (input.length() < T::size) {
			return ParseResult::insufficient_bytes;
		}
		else {
			value = T::read_from_slice(input);
			return ParseResult::ok;
		}
	}

	template <class Spec>
	ParseResult read_enum(RSlice& input, typename Spec::enum_type_t& value)
	{
		if (input.length() < 1) {
			return ParseResult::insufficient_bytes;
		}
		else {
			value = Spec::from_type(input[0]);
			input.advance(1);

			if (value == Spec::enum_type_t::undefined) {
				return ParseResult::undefined_enum;
			}

			return ParseResult::ok;
		}
	}

	ParseResult MessageParser::read(RSlice& input, uint16_t& value)
	{
		return read_integer<UInt16>(input, value);
	}

	ParseResult MessageParser::read(openpal::RSlice& input, uint32_t& value)
	{
		return read_integer<UInt32>(input, value);
	}

	ParseResult MessageParser::read(openpal::RSlice& input, CertificateMode& value)
	{
		return read_enum<CertificateModeSpec>(input, value);
	}

	ParseResult MessageParser::read(openpal::RSlice& input, DHMode& value)
	{
		return read_enum<DHModeSpec>(input, value);
	}
}

