
#ifndef SSP21_TRUNCATED_MAC_SESSION_MODE_H
#define SSP21_TRUNCATED_MAC_SESSION_MODE_H

#include "ssp21/crypto/ISessionMode.h"

#include "ssp21/crypto/CryptoTypedefs.h"

namespace ssp21
{

    class TruncatedMacSessionMode : public ISessionMode
    {

    public:

        TruncatedMacSessionMode(
            mac_func_t const mac_func,
            const uint16_t trunc_length
        ) : mac_func(mac_func), trunc_length(trunc_length)
        {}

        virtual openpal::RSlice read(
            const SymmetricKey& key,
            const AuthMetadata& metadata,
            const openpal::RSlice& payload,
            openpal::WSlice dest,
            std::error_code& ec
        ) override;

        virtual openpal::RSlice write(
            const SymmetricKey& key,
            const AuthMetadata& metadata,
            const openpal::RSlice& userdata,
            openpal::WSlice dest,
            std::error_code& ec
        ) override;

        virtual uint16_t max_writable_user_data_length(uint16_t max_payload_size) override;

    private:

        mac_func_t const mac_func;
        const uint16_t trunc_length;

    };

}

#endif
