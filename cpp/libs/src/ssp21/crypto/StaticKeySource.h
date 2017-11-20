
#ifndef SSP21_STATICKEYSOURCE_H
#define SSP21_STATICKEYSOURCE_H

#include "ssp21/crypto/IKeySource.h"

namespace ssp21
{
    class StaticKeySource final : public IKeySource
    {
    public:

        StaticKeySource(const std::shared_ptr<const SymmetricKey>& key) : key(key)
        {}

        static std::shared_ptr<IKeySource> create(const std::shared_ptr<const SymmetricKey>& key)
        {
            return std::make_shared<StaticKeySource>(key);
        }

        virtual std::shared_ptr<const SymmetricKey> get_key(SecureBuffer& key_id_out) override
        {
            key_id_out.set_type(BufferType::empty);
            return this->key;
        }

    private:
        const std::shared_ptr<const SymmetricKey> key;
    };




}

#endif
