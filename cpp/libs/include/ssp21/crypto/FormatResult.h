#ifndef SSP21_FORMAT_RESULT_H
#define SSP21_FORMAT_RESULT_H

#include "ssp21/SequenceTypes.h"
#include "ssp21/crypto/gen/FormatError.h"

namespace ssp21
{

    struct FormatResult
    {
        static FormatResult Success(const seq32_t& written)
        {
            return FormatResult(FormatError::ok, written);
        }

        explicit FormatResult(FormatError err) : err(err)
        {}

        FormatError err;
        seq32_t written;

        bool is_error() const
        {
            return any(err);
        }

    private:

        FormatResult() = delete;

        FormatResult(FormatError err, const seq32_t& written) :
            err(err),
            written(written)
        {}
    };
}

#endif