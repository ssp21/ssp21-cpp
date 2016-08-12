
#ifndef SSP21_ERROR_CODES_H
#define SSP21_ERROR_CODES_H

#include <system_error>
#include <string>

#if (defined _MSC_VER && (_MSC_VER < 1900))
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif

namespace ssp21
{				
	enum class Error : int
	{
		BAD_KEY_TYPE,
		DH_X25519_FAIL
	};		

	class ErrorCategory final : public std::error_category
	{
	public:

		static const std::error_category& Instance()
		{
			return instance;
		}

		virtual const char* name() const NOEXCEPT
		{
			return "Crypto Errors";
		}
		virtual std::string message(int ev) const;

	private:

		ErrorCategory() {}
		ErrorCategory(const ErrorCategory&) = delete;

		static ErrorCategory instance;
	};

	
	std::error_code make_error_code(Error err);		
}

namespace std
{
	template <>
	struct is_error_code_enum<ssp21::Error> : public true_type{};
}

#endif
