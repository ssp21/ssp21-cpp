
#include "ssp21/ErrorCodes.h"

namespace ssp21
{					

ErrorCategory ErrorCategory::instance;

std::string ErrorCategory::message(int ev) const
{
	return crypto_error_to_string(static_cast<CryptoError>(ev));
}

std::error_code make_error_code(CryptoError err)
{
	return std::error_code(static_cast<int>(err), ErrorCategory::Instance());
}
	
}


