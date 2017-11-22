
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
        * Consume a key from the key source.
        *
        * @return a valid key, or nullptr if no key is available
        */
        virtual std::shared_ptr<const SymmetricKey> consume_key() = 0;
    };




}

#endif
