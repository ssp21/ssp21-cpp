#ifndef SSP21_FORMAT_RESULT_H
#define SSP21_FORMAT_RESULT_H

#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/util/SequenceTypes.h"

namespace ssp21 {

struct FormatResult {
    static FormatResult success(const seq32_t& written)
    {
        return FormatResult(FormatError::ok, written);
    }

    static FormatResult error(FormatError err)
    {
        return FormatResult(err);
    }

    FormatError err;
    seq32_t written;

    bool is_error() const
    {
        return any(err);
    }

protected:
    FormatResult() = delete;

    FormatResult(FormatError err, const seq32_t& written)
        : err(err)
        , written(written)
    {
    }

    explicit FormatResult(FormatError err)
        : err(err)
    {
    }
};
}

#endif