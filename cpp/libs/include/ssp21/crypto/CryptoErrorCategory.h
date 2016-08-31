
#ifndef SSP21_ERROR_CODES_H
#define SSP21_ERROR_CODES_H

#include <system_error>
#include <string>

#include "ssp21/gen/CryptoError.h"

#if (defined _MSC_VER && (_MSC_VER < 1900))
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif

namespace ssp21
{

    class CryptoErrorCategory final : public std::error_category
    {
    public:

        static const std::error_category& get()
        {
            static CryptoErrorCategory instance;
            return instance;
        }

        virtual const char* name() const NOEXCEPT
        {
            return "crypto error";
        }

        virtual std::string message(int ev) const
        {
            return CryptoErrorSpec::to_string(static_cast<CryptoError>(ev));
        }

    private:

        CryptoErrorCategory() {}
        CryptoErrorCategory(const CryptoErrorCategory&) = delete;
    };


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
