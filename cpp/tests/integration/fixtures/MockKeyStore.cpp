#include "MockKeyStore.h"

namespace ssp21
{

    std::shared_ptr<const SymmetricKey> MockKeyStore::find_and_consume_key(uint64_t key_id)
    {
        if (this->current_key)
        {
            if (key_id != this->current_key->id) return nullptr;

            const auto key = this->current_key->key;
            this->current_key.reset();
            return key;
        }
        else
        {
            return nullptr;
        }
    }

    std::shared_ptr<const KeyRecord> MockKeyStore::consume_key()
    {
        SymmetricKey key_data;
        {
            auto dest = key_data.as_wseq();
            Crypto::gen_random(dest);
            key_data.set_type(BufferType::symmetric_key);
        }


        this->current_key = std::make_shared<KeyRecord>(this->key_id, key_data.as_seq());
        ++(this->key_id);
        return this->current_key;
    }

}


