
#ifndef SSP21_KEYRECORD_H
#define SSP21_KEYRECORD_H

#include "ssp21/crypto/BufferTypes.h"

#include <memory>

namespace ssp21
{
    /**
    * symmetric key + 64-bit identifier
    */
    class KeyRecord
    {
    public:

        KeyRecord() = delete;

        KeyRecord(uint64_t id, const ssp21::seq32_t& key);

        const uint64_t id;
        const std::shared_ptr<const SymmetricKey> key;

    private:

        static std::shared_ptr<const SymmetricKey> create_key(const ssp21::seq32_t& key);
    };




}

#endif
