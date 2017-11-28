#ifndef SSP21_MOCKKEYSTORE_H
#define SSP21_MOCKKEYSTORE_H

#include "ssp21/crypto/IKeyLookup.h"
#include "ssp21/crypto/IKeySource.h"
#include "ssp21/crypto/Crypto.h"

namespace ssp21
{
    class MockKeyStore : public IKeyLookup, public IKeySource
    {

    public:

        virtual std::shared_ptr<const SymmetricKey> find_and_consume_key(uint64_t key_id) override;

        virtual std::shared_ptr<const KeyRecord> consume_key() override;

    private:

        uint64_t key_id = 0;

        std::shared_ptr<const KeyRecord> current_key;

    };

}

#endif
