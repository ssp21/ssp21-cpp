#include "ssp21/stack/Version.h"

#define STRINGIFY(s) #s
#define TO_STRING(s) STRINGIFY(s)

#include <cstring>

namespace ssp21
{
    namespace version
    {
        const char* git_commit_hash()
        {
#ifndef MACRO_SSP21_GIT_COMMIT_HASH
            static_assert(false, "MACRO_SSP21_GIT_COMMIT_HASH not defined");
#endif
            
            return TO_STRING(MACRO_SSP21_GIT_COMMIT_HASH);
        }

        const char* git_commit_date()
        {
#ifndef MACRO_SSP21_GIT_COMMIT_HASH
            static_assert(false, "MACRO_SSP21_GIT_COMMIT_DATE not defined");
#endif
            return TO_STRING(MACRO_SSP21_GIT_COMMIT_DATE);
        }
    }
}
