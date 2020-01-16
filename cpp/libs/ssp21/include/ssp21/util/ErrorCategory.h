
#ifndef SSP21_ERRORCATEGORY_H
#define SSP21_ERRORCATEGORY_H

#include <string>
#include <system_error>

#if (defined _MSC_VER && (_MSC_VER < 1900))
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif

namespace ssp21 {

template <class EnumSpec>
class ErrorCategory final : public std::error_category {
public:
    static const std::error_category& get()
    {
        static ErrorCategory instance;
        return instance;
    }

    virtual const char* name() const NOEXCEPT
    {
        return EnumSpec::name;
    }

    virtual std::string message(int ev) const
    {
        return EnumSpec::to_string(static_cast<typename EnumSpec::enum_type_t>(ev));
    }

private:
    ErrorCategory() {}
    ErrorCategory(const ErrorCategory&) = delete;
};
}

#endif
