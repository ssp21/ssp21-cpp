
#include "ssp21/crypto/KeyRecord.h"

namespace ssp21
{

    KeyRecord::KeyRecord(uint64_t id, const ssp21::seq32_t& key) : id(id), key(create_key(key))
    {}

    std::shared_ptr<const SymmetricKey> KeyRecord::create_key(const ssp21::seq32_t& key)
    {
        if (key.length() != ssp21::consts::crypto::symmetric_key_length) return nullptr;
        else
        {
            auto ret = std::make_shared<SymmetricKey>();
            ret->as_wseq().copy_from(key);
            ret->set_type(BufferType::symmetric_key);
            return ret;
        }
    }

}

