#ifndef SSP21_MESSAGE_FORMATTER_H
#define SSP21_MESSAGE_FORMATTER_H

#include "ssp21/crypto/gen/Function.h"

#include "ssp21/crypto/FormatResult.h"
#include "ssp21/crypto/EnumField.h"

#include "openpal/util/Uncopyable.h"

namespace ssp21
{

    class MessageFormatter : private openpal::StaticOnly
    {

    public:

        template <typename WriteFun>
        static FormatResult write_message(wseq32_t& dest, Function function, const WriteFun& write_fields)
        {
            const auto start = dest;

            EnumField<FunctionSpec> func(function);
            auto ferr = func.write(dest);
            if (any(ferr)) return FormatResult::error(ferr);

            auto merr = write_fields(dest);
            if (any(merr)) return FormatResult::error(merr);

            const auto num_written = start.length() - dest.length();
            return FormatResult::success(start.readonly().take(num_written));
        }

        template <typename T, typename... Args>
        static FormatError write_fields(wseq32_t& dest, const T& value, Args& ... args)
        {
            auto err = value.write(dest);
            if (any(err)) return err;
            return write_fields(dest, args...);
        }

    private:

        static FormatError write_fields(wseq32_t& output)
        {
            return FormatError::ok;
        }

    };
}

#endif