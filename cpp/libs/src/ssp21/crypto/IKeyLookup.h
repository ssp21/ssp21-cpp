
#ifndef SSP21_IKEYLOOKUP_H
#define SSP21_IKEYLOOKUP_H

#include "ssp21/crypto/BufferTypes.h"

#include <memory>

namespace ssp21
{
    /**
    * Interface used to lookup a shared secret key using a key identifier
    */
    class IKeyLookup
    {
    public:

        virtual ~IKeyLookup() {}

        /**
        * Try to find a key using an identifier. Static key schemes expect the key identifier to be empty
        * and always return the same key.
        *
        * @param key_id_out An optional key identifier set if the key is a rotating key.
        *
        * @return a valid key, or nullptr if no key is available w/ the specified id
        */
        std::shared_ptr<const SymmetricKey> find_key(const seq32_t& key_id);
    };




}

#endif
