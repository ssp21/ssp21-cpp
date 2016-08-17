#ifndef SSP21_MESSAGE_FORMATTER_H
#define SSP21_MESSAGE_FORMATTER_H

#include "ssp21/gen/Function.h"
#include "ssp21/gen/CertificateMode.h"
#include "ssp21/gen/DHMode.h"
#include "ssp21/gen/HandshakeError.h"
#include "ssp21/gen/NonceMode.h"
#include "ssp21/gen/SessionMode.h"
#include "ssp21/gen/HashMode.h"
#include "ssp21/gen/FormatError.h"

#include "ssp21/SequenceTypes.h"

#include "openpal/container/WSlice.h"
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

	class MessageFormatter : private openpal::StaticOnly
	{

	public:

		template <Function function, typename T, typename... Args>
		static FormatError write_message(openpal::WSlice& dest, const T& value, Args& ... args)
		{			
			auto err = write(dest, function);
			if (any(err)) return err;

			return write_fields(dest, value, args...);
		}	

		template <typename T, typename... Args>
		static FormatError write_fields(openpal::WSlice& dest, const T& value, Args& ... args)
		{
			auto err = write(dest, value);
			if (any(err)) return err;
			return write_fields(dest, args...);
		}
		
		static FormatError write_fields(openpal::WSlice& output) {
			return FormatError::ok;
		}		

		// integers
		static FormatError write(openpal::WSlice& dest, uint8_t value);
		static FormatError write(openpal::WSlice& dest, uint16_t value);
		static FormatError write(openpal::WSlice& dest, uint32_t value);

		// enums
		static FormatError write(openpal::WSlice& dest, Function value);
		static FormatError write(openpal::WSlice& dest, CertificateMode value);
		static FormatError write(openpal::WSlice& dest, DHMode value);
		static FormatError write(openpal::WSlice& dest, HandshakeError value);
		static FormatError write(openpal::WSlice& dest, NonceMode value);
		static FormatError write(openpal::WSlice& dest, SessionMode value);
		static FormatError write(openpal::WSlice& dest, HashMode value);

		// sequences
		static FormatError write(openpal::WSlice& dest, const Seq8& value);
		static FormatError write(openpal::WSlice& dest, const Seq16& value);
		static FormatError write(openpal::WSlice& dest, const Seq8Seq16& value);


		template <class WriteFunc>
		static FormatError write_any_with_written(const WriteFunc& write, openpal::WSlice& dest, openpal::RSlice& written)
		{
			const auto start = dest;
			auto err = write(dest);
			if (any(err)) {
				written = openpal::RSlice::empty_slice();
				return err;
			}
			else {
				const auto num_written = start.length() - dest.length();
				written = start.as_rslice().take(num_written);
				return err;
			}
		}
		
		
	};
}

#endif