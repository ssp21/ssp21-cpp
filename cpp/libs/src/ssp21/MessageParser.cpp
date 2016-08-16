
#include "ssp21/MessageParser.h"

#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace ssp21 {

	template <class T>
	ParseError read_integer(RSlice& input, typename T::type_t& value)
	{
		if (input.length() < T::size) {
			return ParseError::insufficient_bytes;
		}
		else {
			value = T::read_from_slice(input);
			return ParseError::ok;
		}
	}

	template <class Spec>
	ParseError read_enum(RSlice& input, typename Spec::enum_type_t& value)
	{
		if (input.length() < 1) {
			return ParseError::insufficient_bytes;
		}
		else {
			value = Spec::from_type(input[0]);
			input.advance(1);

			if (value == Spec::enum_type_t::undefined) {
				return ParseError::undefined_enum;
			}

			return ParseError::ok;
		}
	}

	template <class CountType, class SeqType>
	ParseError read_seq(openpal::RSlice& input, SeqType& value)
	{
		typename CountType::type_t count;
		auto result = read_integer<CountType>(input, count);
		if (result != ParseError::ok) {
			return result;
		}

		if (input.length() < count) {
			return ParseError::insufficient_bytes;
		}

		value = SeqType(input.take(count));
		input.advance(count);
		return ParseError::ok;
	}

	ParseError MessageParser::read(openpal::RSlice& input, uint8_t& value)
	{
		return read_integer<UInt8>(input, value);
	}

	ParseError MessageParser::read(RSlice& input, uint16_t& value)
	{
		return read_integer<UInt16>(input, value);
	}

	ParseError MessageParser::read(openpal::RSlice& input, uint32_t& value)
	{
		return read_integer<UInt32>(input, value);
	}

	ParseError MessageParser::read(openpal::RSlice& input, CertificateMode& value)
	{
		return read_enum<CertificateModeSpec>(input, value);
	}

	ParseError MessageParser::read(openpal::RSlice& input, DHMode& value)
	{
		return read_enum<DHModeSpec>(input, value);
	}

	ParseError MessageParser::read(openpal::RSlice& input, HandshakeError& value)
	{
		return read_enum<HandshakeErrorSpec>(input, value);
	}
	
	ParseError MessageParser::read(openpal::RSlice& input, NonceMode& value)
	{
		return read_enum<NonceModeSpec>(input, value);
	}
	
	ParseError MessageParser::read(openpal::RSlice& input, SessionMode& value)
	{
		return read_enum<SessionModeSpec>(input, value);
	}
	
	ParseError MessageParser::read(openpal::RSlice& input, HashMode& value)
	{
		return read_enum<HashModeSpec>(input, value);
	}

	ParseError MessageParser::read(openpal::RSlice& input, Seq8& value)
	{
		return read_seq<UInt8, Seq8>(input, value);		
	}
	
	ParseError MessageParser::read(openpal::RSlice& input, Seq16& value)
	{
		return read_seq<UInt16, Seq16>(input, value);
	}
	
	ParseError MessageParser::read(openpal::RSlice& input, Seq8Seq16& value)
	{
		value.clear();

		uint8_t count;
		
		auto cresult = read(input, count);
		if (cresult != ParseError::ok) {
			return cresult;
		}
		
		while (count > 0) {
			Seq16 slice;
			auto sresult = read(input, slice);
			if (sresult != ParseError::ok) {
				return sresult;
			}
			if (!value.push(slice)) {
				return ParseError::impl_capacity_limit;
			}
			--count;
		}		
		
		return ParseError::ok;
	}
}

