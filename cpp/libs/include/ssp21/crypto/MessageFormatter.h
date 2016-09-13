#ifndef SSP21_MESSAGE_FORMATTER_H
#define SSP21_MESSAGE_FORMATTER_H

#include "ssp21/crypto/gen/Function.h"

#include "ssp21/crypto/FormatResult.h"
#include "ssp21/crypto/EnumField.h"

#include "openpal/container/WSlice.h"
#include "openpal/util/Uncopyable.h"

namespace ssp21
{

    class MessageFormatter : private openpal::StaticOnly
    {

    public:

        template <typename WriteFun>
        static FormatResult write_message(openpal::WSlice& dest, EnumField<FunctionSpec> function, const WriteFun& write_fields)
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

        // anything with a write method
		template <class T>
        inline static FormatError write(openpal::WSlice& dest, const T& field)
        {
            return field.write(dest);
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