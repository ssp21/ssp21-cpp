#ifndef SSP21_FORMAT_RESULT_H
#define SSP21_FORMAT_RESULT_H

#include "openpal/container/RSlice.h"
#include "openpal/container/WSlice.h"

#include "ssp21/crypto/gen/FormatError.h"

namespace ssp21
{

    struct FormatResult
    {
        static FormatResult Error(FormatError err)
        {
            return FormatResult(err, openpal::RSlice::empty_slice());
        }

        static FormatResult Succes(const openpal::RSlice& written)
        {
            return FormatResult(FormatError::ok, written);
        }

        FormatError err;
        openpal::RSlice written;

        bool is_error() const
        {
            return any(err);
        }

    private:

        FormatResult() = delete;

        FormatResult(FormatError err, const openpal::RSlice& written) :
            err(err),
            written(written)
        {}
    };
}

#endif