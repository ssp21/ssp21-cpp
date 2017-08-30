
#ifndef SSP21_TRUNCATED_MAC_SESSION_MODE_H
#define SSP21_TRUNCATED_MAC_SESSION_MODE_H

#include "ssp21/crypto/ISessionMode.h"

#include "ssp21/crypto/CryptoTypedefs.h"

namespace ssp21
{

    class TruncatedMacSessionMode : public ISessionMode
    {

    public:

        TruncatedMacSessionMode(mac_func_t mac_func, uint8_t auth_tag_length) :
            mac_func(mac_func),
            auth_tag_length(auth_tag_length)
        {}

    private:

        virtual seq32_t read_impl(
            const SymmetricKey& key,
            const SessionData& msg,
            wseq32_t dest,
            std::error_code& ec
        ) const override;

        virtual seq32_t write_impl(
            IFrameWriter& writer,
            const SymmetricKey& key,
            const AuthMetadata& metadata,
            seq32_t& user_data,
            std::error_code& ec
        ) const override;

        const mac_func_t mac_func;
        const uint8_t auth_tag_length;
    };

}

#endif
