
#ifndef SSP21_HANDSHAKE_AUTHENTICATION_H
#define SSP21_HANDSHAKE_AUTHENTICATION_H

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/CryptoTypedefs.h"

#include "openpal/util/Uncopyable.h"

namespace ssp21
{
    enum class EntityId : uint8_t
    {
        Initiator,
        Responder
    };

    typedef bool (*auth_handshake_t)(const SymmetricKey& key, EntityId id, const openpal::RSlice& mac);

    typedef void (*calc_handshake_mac_t)(const SymmetricKey& key, EntityId id, HashOutput& output);



    class HandshakeAuthentication : private openpal::StaticOnly
    {

    public:

        static auth_handshake_t default_auth_handshake()
        {
            return &auth_handshake_hmac_sha256;
        }

        static calc_handshake_mac_t default_calc_handshake_mac()
        {
            return &calc_handshake_hmac_sha256;
        }

        static bool auth_handshake_hmac_sha256(const SymmetricKey& key, EntityId id, const openpal::RSlice& mac_value)
        {
            return auth_handshake_with_mac(&Crypto::hmac_sha256, key, id, mac_value);
        }

        static void calc_handshake_hmac_sha256(const SymmetricKey& key, EntityId id, HashOutput& output)
        {
            return calc_handshake_mac_with_macfunc(&Crypto::hmac_sha256, key, id, output);
        }

    private:

        inline static uint8_t get_calc_byte(EntityId id)
        {
            return (id == EntityId::Initiator) ? 0x01 : 0x02;
        }

        inline static uint8_t get_auth_byte(EntityId id)
        {
            return (id == EntityId::Responder) ? 0x01 : 0x02;
        }

        static bool auth_handshake_with_mac(mac_func_t mac_func, const SymmetricKey& key, EntityId id, const openpal::RSlice& mac_value);

        static void calc_handshake_mac_with_macfunc(mac_func_t mac_func, const SymmetricKey& key, EntityId id, HashOutput& output);
    };
}

#endif