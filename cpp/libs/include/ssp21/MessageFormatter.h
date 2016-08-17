#ifndef SSP21_MESSAGE_FORMATTER_H
#define SSP21_MESSAGE_FORMATTER_H

#include "ssp21/gen/Function.h"
#include "ssp21/gen/CertificateMode.h"
#include "ssp21/gen/DHMode.h"
#include "ssp21/gen/HandshakeError.h"
#include "ssp21/gen/NonceMode.h"
#include "ssp21/gen/SessionMode.h"
#include "ssp21/gen/HashMode.h"

#include "ssp21/SequenceTypes.h"

#include "openpal/container/WSlice.h"
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

	class MessageFormatter : private openpal::StaticOnly
	{

	public:

		template <Function function, typename T, typename... Args>
		static bool write_message(openpal::WSlice& input, const T& value, Args& ... args)
		{			
			auto result = write(input, function);
			if (!result) return result;

			return read_fields(input, value, args...);
		}	

		template <typename T, typename... Args>
		static bool write_fields(openpal::WSlice& dest, const T& value, Args& ... args)
		{
			auto result = write(input, value);
			if (!result) return result;
			return write_fields(input, args...);
		}

		
		static bool write_fields(openpal::WSlice& output) {
			return true;
		}		

		// integers
		static bool write(openpal::WSlice& dest, uint8_t value);
		static bool write(openpal::WSlice& dest, uint16_t value);
		static bool write(openpal::WSlice& dest, uint32_t value);

		// enums
		static bool write(openpal::WSlice& dest, Function value);
		static bool write(openpal::WSlice& dest, CertificateMode value);
		static bool write(openpal::WSlice& dest, DHMode value);
		static bool write(openpal::WSlice& dest, HandshakeError value);
		static bool write(openpal::WSlice& dest, NonceMode value);
		static bool write(openpal::WSlice& dest, SessionMode value);
		static bool write(openpal::WSlice& dest, HashMode value);

		// sequences
		static bool write(openpal::WSlice& dest, const Seq8& value);
		static bool write(openpal::WSlice& dest, const Seq16& value);
		static bool write(openpal::WSlice& dest, const Seq8Seq16& value);
	};
}

#endif