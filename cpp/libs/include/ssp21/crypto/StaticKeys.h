
#ifndef SSP21_STATICKEYS_H
#define SSP21_STATICKEYS_H

#include "ssp21/crypto/BufferTypes.h"

#include <memory>

namespace ssp21
{
    struct StaticKeys
    {
        StaticKeys(
            const std::shared_ptr<const PublicKey>& public_key,
            const std::shared_ptr<const PrivateKey>& private_key
        ) :
            public_key(public_key),
            private_key(private_key)
        {}

        const std::shared_ptr<const PublicKey> public_key;
        const std::shared_ptr<const PrivateKey> private_key;
    };
}

#endif
