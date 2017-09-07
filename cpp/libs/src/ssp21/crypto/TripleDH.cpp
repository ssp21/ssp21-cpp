
#include "TripleDH.h"

namespace ssp21
{

	TripleDH::Result TripleDH::compute(
        dh_func_t dh,
        const StaticKeys& static_keys,
        const KeyPair& ephemeral_keys,
        const seq32_t& remote_public_static,
        const seq32_t& remote_public_ephemeral,
        std::error_code& ec
    )
    {
        dh(ephemeral_keys.private_key, remote_public_ephemeral, dh1, ec);
        if (ec) return Result();

        dh(ephemeral_keys.private_key, remote_public_static, dh2, ec);
        if (ec) return Result();

        dh(*static_keys.private_key, remote_public_ephemeral, dh3, ec);
        if (ec) return Result();

		return Result(dh1.as_seq(), dh2.as_seq(), dh3.as_seq());
    }

}


