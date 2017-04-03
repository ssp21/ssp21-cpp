#ifndef SSP21PROXY_EXCEPTIONMACROS_H
#define SSP21PROXY_EXCEPTIONMACROS_H

#include <stdexcept>
#include <sstream>

#define THROW_LOGIC_ERR(build_pattern, section) { \
    std::ostringstream oss; \
    oss << build_pattern << " [section = " << section << "]"; \
    throw std::logic_error(oss.str()); \
}

#endif

