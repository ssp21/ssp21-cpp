#ifndef SSP21_MESSAGE_PARSER_H
#define SSP21_MESSAGE_PARSER_H

#include "ssp21/gen/ParseResult.h"

#include "ssp21/gen/Function.h"
#include "ssp21/gen/CertificateMode.h"
#include "ssp21/gen/DHMode.h"
#include "ssp21/gen/HandshakeError.h"
#include "ssp21/gen/NonceMode.h"
#include "ssp21/gen/SessionMode.h"
#include "ssp21/gen/HashMode.h"

#include "ssp21/SequenceTypes.h"

#include "openpal/container/RSlice.h"
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

	class MessageParser : private openpal::StaticOnly
	{
		
		template <Function expected, typename T, typename... Args>
		static ParseResult read_message(openpal::RSlice& input, T& value, Args& ... args)
		{
			Function func;
			auto result = read(input, func);
			if (ret != ParseResult::ok) return ret;
			if (func != expected) return ParseResult::unexpected_function;

			return read_fields(input, value, args...)
		}

	private:

		template <typename T, typename... Args>
		static ParseResult read_fields(openpal::RSlice& input, T& value, Args& ... args)
		{
			auto ret = read(input, value);
			if (ret != ParseResult::ok) return ret;
			return read_fields(input, args...);
		}

		// when we reach the base case of the recursion there shouldn't be any remaining data
		static ParseResult read_fields(openpal::RSlice& input) {
			return input.is_empty() ? ParseResult::ok : ParseResult::too_many_bytes;
		}
		

		// integers
		static ParseResult read(openpal::RSlice& input, uint8_t& value);
		static ParseResult read(openpal::RSlice& input, uint16_t& value);
		static ParseResult read(openpal::RSlice& input, uint32_t& value);			

		// enums
		static ParseResult read(openpal::RSlice& input, CertificateMode& value);
		static ParseResult read(openpal::RSlice& input, DHMode& value);
		static ParseResult read(openpal::RSlice& input, HandshakeError& value);
		static ParseResult read(openpal::RSlice& input, NonceMode& value);
		static ParseResult read(openpal::RSlice& input, SessionMode& value);
		static ParseResult read(openpal::RSlice& input, HashMode& value);

		// sequences
		static ParseResult read(openpal::RSlice& input, Seq8& value);
		static ParseResult read(openpal::RSlice& input, Seq16& value);
		static ParseResult read(openpal::RSlice& input, Seq8Seq16& value);
	};
}

#endif