
#ifndef SSP21_ERROR_CODES_H
#define SSP21_ERROR_CODES_H

#include "ssp21/ErrorCategory.h"
#include "ssp21/crypto/gen/CryptoError.h"

namespace ssp21
{
	typedef ErrorCategory<CryptoErrorSpec> CryptoErrorCategory;

    inline std::error_code make_error_code(CryptoError err)
    {
        return std::error_code(static_cast<int>(err), CryptoErrorCategory::get());
    }
}

namespace std
{
    template <>
    struct is_error_code_enum<ssp21::CryptoError> : public true_type {};
}

#endif
