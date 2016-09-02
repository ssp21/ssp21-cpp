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

#include "ssp21/crypto/FormatResult.h"
#include "ssp21/crypto/SequenceTypes.h"
#include "ssp21/crypto/IWritable.h"

#include "openpal/container/WSlice.h"
#include "openpal/util/Uncopyable.h"

namespace ssp21
{

    class MessageFormatter : private openpal::StaticOnly
    {

    public:

        template <typename WriteFun>
        static FormatResult write_message(openpal::WSlice& dest, Function function, const WriteFun& write_fields)
        {
            const auto start = dest;

            auto ferr = write(dest, function);
            if (any(ferr)) return FormatResult::Error(ferr);

            auto merr = write_fields(dest);
            if (any(merr)) return FormatResult::Error(merr);

            const auto num_written = start.length() - dest.length();
            return FormatResult::Succes(start.as_rslice().take(num_written));
        }

        template <typename T, typename... Args>
        static FormatError write_fields(openpal::WSlice& dest, const T& value, Args& ... args)
        {
            auto err = write(dest, value);
            if (any(err)) return err;
            return write_fields(dest, args...);
        }

        static FormatError write_fields(openpal::WSlice& output)
        {
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

        // any writable
        static FormatError write(openpal::WSlice& dest, const IWritable& value)
        {
            return value.write(dest);
        }

        template <class WriteFunc>
        static FormatError write_any_with_written(const WriteFunc& write, openpal::WSlice& dest, openpal::RSlice& written)
        {
            const auto start = dest;
            auto err = write(dest);
            if (any(err))
            {
                written = openpal::RSlice::empty_slice();
                return err;
            }
            else
            {
                const auto num_written = start.length() - dest.length();
                written = start.as_rslice().take(num_written);
                return err;
            }
        }


    };
}

#endif