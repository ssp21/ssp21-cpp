

#include "ssp21/crypto/HandshakeAuthentication.h"

namespace ssp21
{

    bool HandshakeAuthentication::auth_handshake_with_mac(mac_func_t mac_func, const SymmetricKey& key, EntityId id, const Seq8& mac_value)
    {
        auto byte = get_auth_byte(id);
        const openpal::RSlice pattern(&byte, 1);
        HashOutput mac_buffer;
        mac_func(key.as_slice().widen(), { pattern }, mac_buffer);
        return Crypto::secure_equals(mac_buffer.as_slice(), mac_value);
    }

    void HandshakeAuthentication::calc_handshake_mac_with_macfunc(mac_func_t mac_func, const SymmetricKey& key, EntityId id, HashOutput& output)
    {
        auto byte = get_calc_byte(id);
        const openpal::RSlice pattern(&byte, 1);
        mac_func(key.as_slice().widen(), { pattern }, output);
    }
}


