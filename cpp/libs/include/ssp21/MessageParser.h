#ifndef SSP21_MESSAGE_PARSER_H
#define SSP21_MESSAGE_PARSER_H

#include "ssp21/gen/ParseResult.h"

#include "openpal/container/RSlice.h"
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

	class MessageParser : private openpal::StaticOnly
	{


	private:

		static ParseResult read(openpal::RSlice& input, uint16_t& value);
		static ParseResult read(openpal::RSlice& input, uint32_t& value);		
	};
}

#endif