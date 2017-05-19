
#ifndef SSP21_SESSION_MODES_H
#define SSP21_SESSION_MODES_H

#include "TruncatedMacSessionMode.h"

namespace ssp21
{
    class SessionModes : private openpal::StaticOnly
    {

    public:

        static ISessionMode& get_hmac_sha_256_trunc16()
        {
            return hmac_sha_256_trunc16;
        }

        static ISessionMode& default_mode()
        {
            return hmac_sha_256_trunc16;
        }

    private:

        static TruncatedMacSessionMode hmac_sha_256_trunc16;
    };

}

#endif
