#ifndef SSP21_FORMAT_RESULT_H
#define SSP21_FORMAT_RESULT_H

#include "openpal/container/RSlice.h"
#include "openpal/container/WSlice.h"

#include "ssp21/gen/FormatError.h"

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

    template <class WriteFunc>
    static FormatResult write_any(const WriteFunc& write, openpal::WSlice& dest)
    {
        const auto start = dest;
        auto err = write(dest);
        if (any(err))
        {
            return FormatResult::Error(err);
        }
        else
        {
            const auto num_written = start.length() - dest.length();
            return FormatResult::Succes(start.as_rslice().take(num_written));
        }
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