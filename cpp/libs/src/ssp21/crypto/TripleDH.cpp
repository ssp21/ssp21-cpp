
#include "TripleDH.h"

namespace ssp21
{

    std::initializer_list<seq32_t> TripleDH::compute(
        dh_func_t dh,
        const StaticKeys& static_keys,
        const KeyPair& ephemeral_keys,
        const seq32_t& remote_public_static,
        const seq32_t& remote_public_ephemeral,
        std::error_code& ec
    )
    {
        dh(ephemeral_keys.private_key, remote_public_ephemeral, dh1, ec);
        if (ec) return {};

        dh(ephemeral_keys.private_key, remote_public_static, dh2, ec);
        if (ec) return {};

        dh(*static_keys.private_key, remote_public_ephemeral, dh3, ec);
        if (ec) return {};

        return { dh1.as_seq(), dh2.as_seq(), dh3.as_seq() };
    }

}


