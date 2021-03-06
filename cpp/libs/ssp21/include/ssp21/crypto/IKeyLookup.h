
#ifndef SSP21_IKEYLOOKUP_H
#define SSP21_IKEYLOOKUP_H

#include "ssp21/crypto/BufferTypes.h"

#include <memory>

namespace ssp21 {
/**
    * Interface used to lookup a shared secret key using a key identifier
    */
class IKeyLookup {
public:
    virtual ~IKeyLookup() {}

    /**
        * Try to find a key using an identifier
        *
        * @param key_id The key identifier if the key is a rotating key.
        *
        * @return a valid key, or nullptr if no key is available w/ the specified id
        */
    virtual std::shared_ptr<const SymmetricKey> find_and_consume_key(uint64_t key_id) = 0;
};

}

#endif
