#include "MockKeyStore.h"

namespace ssp21
{

    std::shared_ptr<const SymmetricKey> MockKeyStore::find_and_consume_key(const seq32_t& key_id)
    {
        if (this->current_key)
        {
            // calc the SHA-2 hash of the key
            HashOutput hash;
            Crypto::hash_sha256({ this->current_key->as_seq() }, hash);
            if (!hash.as_seq().equals(key_id)) return nullptr;
            const auto ret = this->current_key;
            this->current_key.reset();
            return ret;
        }
        else
        {
            return nullptr;
        }
    }

    std::shared_ptr<const SymmetricKey> MockKeyStore::consume_key()
    {
        auto key = std::make_shared<SymmetricKey>();

        auto dest = key->as_wseq();
        Crypto::gen_random(dest);
        key->set_type(BufferType::symmetric_key);

        this->current_key = key;

        return key;
    }

}


