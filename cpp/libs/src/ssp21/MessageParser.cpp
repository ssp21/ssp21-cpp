
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

	template <class CountType, class SeqType>
	ParseResult read_seq(openpal::RSlice& input, SeqType& value)
	{
		typename CountType::type_t count;
		auto result = read_integer<CountType>(input, count);
		if (result != ParseResult::ok) {
			return result;
		}

		if (input.length() < count) {
			return ParseResult::insufficient_bytes;
		}

		value = SeqType(input.take(count));
		input.advance(count);
		return ParseResult::ok;
	}

	ParseResult MessageParser::read(openpal::RSlice& input, uint8_t& value)
	{
		return read_integer<UInt8>(input, value);
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

	ParseResult MessageParser::read(openpal::RSlice& input, HandshakeError& value)
	{
		return read_enum<HandshakeErrorSpec>(input, value);
	}
	
	ParseResult MessageParser::read(openpal::RSlice& input, NonceMode& value)
	{
		return read_enum<NonceModeSpec>(input, value);
	}
	
	ParseResult MessageParser::read(openpal::RSlice& input, SessionMode& value)
	{
		return read_enum<SessionModeSpec>(input, value);
	}
	
	ParseResult MessageParser::read(openpal::RSlice& input, HashMode& value)
	{
		return read_enum<HashModeSpec>(input, value);
	}

	ParseResult MessageParser::read(openpal::RSlice& input, Seq8& value)
	{
		return read_seq<UInt8, Seq8>(input, value);		
	}
	
	ParseResult MessageParser::read(openpal::RSlice& input, Seq16& value)
	{
		return read_seq<UInt16, Seq16>(input, value);
	}
	
	ParseResult MessageParser::read(openpal::RSlice& input, Seq8Seq16& value)
	{
		value.clear();

		uint8_t count;
		
		auto cresult = read(input, count);
		if (cresult != ParseResult::ok) {
			return cresult;
		}
		
		while (count > 0) {
			Seq16 slice;
			auto sresult = read(input, slice);
			if (sresult != ParseResult::ok) {
				return sresult;
			}
			if (!value.push(slice)) {
				return ParseResult::impl_capacity_limit;
			}
			--count;
		}		
		
		return ParseResult::ok;
	}
}

