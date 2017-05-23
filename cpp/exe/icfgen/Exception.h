
#ifndef ICFGEN_EXCEPTION_H
#define ICFGEN_EXCEPTION_H

#include <stdexcept>
#include "ssp21/util/StringUtil.h"

class Exception : public std::runtime_error
{

public:

    template <class T, class... Args>
    Exception(T t, Args... args) :
        std::runtime_error(ssp21::strings::join(t, args...))
    {

    }

};

#endif