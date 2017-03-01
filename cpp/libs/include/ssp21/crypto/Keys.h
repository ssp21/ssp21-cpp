
#ifndef SSP21_KEYS_H
#define SSP21_KEYS_H

#include "ssp21/crypto/BufferTypes.h"

#include <memory>

namespace ssp21
{
    struct Keys
    {
        Keys(
            const std::shared_ptr<const PublicKey> local_static_public_key,
            const std::shared_ptr<const PublicKey> remote_static_public_key,
            const std::shared_ptr<const PrivateKey> local_static_private_key
        ) :
            local_static_public_key(local_static_public_key),
            remote_static_public_key(remote_static_public_key),
            local_static_private_key(local_static_private_key)
        {}

        const std::shared_ptr<const PublicKey> local_static_public_key;
        const std::shared_ptr<const PublicKey> remote_static_public_key;
        const std::shared_ptr<const PrivateKey> local_static_private_key;
    };
}

#endif
