#ifndef SSP21_MESSAGE_PARSER_H
#define SSP21_MESSAGE_PARSER_H

#include "ssp21/gen/ParseResult.h"


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


	private:

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