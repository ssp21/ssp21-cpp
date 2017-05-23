
#ifndef SSP21_EXCEPTION_H
#define SSP21_EXCEPTION_H

#include <stdexcept>
#include "ssp21/util/StringUtil.h"

namespace ssp21
{
    /**
    * Super class of std::runtime_error that allows
    * message to be built dynamically
    */
    class Exception : public std::runtime_error
    {

    public:

        Exception(const char* message) : std::runtime_error(message)
        {

        }

        template <class T, class... Args>
        Exception(T t, Args... args) :
            std::runtime_error(ssp21::strings::join(t, args...))
        {

        }

    };
}


#endif