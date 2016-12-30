
#ifndef SSP21_TRUNCATED_MAC_SESSION_MODE_H
#define SSP21_TRUNCATED_MAC_SESSION_MODE_H

#include "ssp21/crypto/ISessionMode.h"

#include "ssp21/crypto/CryptoTypedefs.h"

namespace ssp21
{

    class TruncatedMacSessionMode : public ISessionMode
    {

    public:

        TruncatedMacSessionMode(mac_func_t mac_func, uint8_t auth_tag_length, BufferType buffer_type) :
            mac_func(mac_func),
            auth_tag_length(auth_tag_length),
            buffer_type(buffer_type)
        {}

        virtual seq16_t read(
            const SymmetricKey& key,
            const SessionData& msg,
            wseq32_t dest,
            std::error_code& ec
        ) const override;

        virtual seq16_t write(
            const SymmetricKey& key,
            const AuthMetadata& metadata,
            const seq16_t& user_data,
            AuthenticationTag& auth_tag,
            wseq32_t dest,
            std::error_code& ec
        ) const override;

        virtual uint16_t max_writable_user_data_length(uint16_t max_payload_size) const override;

    private:

        mac_func_t const mac_func;
        const uint8_t auth_tag_length;
        BufferType buffer_type;

    };

}

#endif
