
#ifndef SSP21PROXY_SECTIONEXCEPTION_H
#define SSP21PROXY_SECTIONEXCEPTION_H

#include "ssp21/util/Exception.h"

/**
* Dynamically build exception messages that include an explicit section name
*/
class SectionException : public ssp21::Exception
{

public:

    template <class... Args>
    SectionException(const std::string& section, Args... args) :
        ssp21::Exception(args..., " in section: ", section)
    {

    }

};


#endif