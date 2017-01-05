
#ifndef SSP21_HANDSHAKE_AUTHENTICATION_H
#define SSP21_HANDSHAKE_AUTHENTICATION_H

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/CryptoTypedefs.h"

#include "openpal/util/Uncopyable.h"

namespace ssp21
{
    enum class EntityType : uint8_t
    {
        Initiator,
        Responder
    };

    typedef bool (*auth_handshake_t)(const SymmetricKey& key, EntityType id, const seq32_t& mac);

    typedef void (*calc_handshake_mac_t)(const SymmetricKey& key, EntityType id, HashOutput& output);



    class HandshakeAuthentication : private openpal::StaticOnly
    {

    public:

        static bool auth_handshake_with_mac(mac_func_t mac_func, const SymmetricKey& key, EntityType id, const seq8_t& mac_value);

        static void calc_handshake_mac_with_macfunc(mac_func_t mac_func, const SymmetricKey& key, EntityType id, HashOutput& output);

    private:

        inline static uint8_t get_calc_byte(EntityType id)
        {
            return (id == EntityType::Initiator) ? 0x01 : 0x02;
        }

        inline static uint8_t get_auth_byte(EntityType id)
        {
            return (id == EntityType::Responder) ? 0x01 : 0x02;
        }


    };
}

#endif
