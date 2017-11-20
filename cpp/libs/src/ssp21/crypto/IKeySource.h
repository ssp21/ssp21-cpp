
#ifndef SSP21_IKEYSOURCE_H
#define SSP21_IKEYSOURCE_H

#include "ssp21/crypto/BufferTypes.h"

#include <memory>

namespace ssp21
{
    /**
    * Interface used to lookup
    */
    class IKeySource
    {
    public:

        virtual ~IKeySource() {}

        /**
        * Try to get a key from the key source. This could be the same static key every time, or it might consume a key from a store of keys.
        *
        * @param key_id_out An optional key identifier set if the key is a rotating key.
        *
        * @return a valid key, or nullptr if no key is available
        */
        std::shared_ptr<const SymmetricKey> get_key(SecureBuffer& key_id_out);
    };




}

#endif
