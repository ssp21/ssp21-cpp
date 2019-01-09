
#ifndef SSP21_SECUREDYNAMICBUFFER_H
#define SSP21_SECUREDYNAMICBUFFER_H

#include "ser4cpp/container/Buffer.h"

#include "ssp21/crypto/Crypto.h"


namespace ssp21
{

    /**
    * A dynamic buffer that clears itself when deleted
    */
    class SecureDynamicBuffer final : public ser4cpp::Buffer
    {

    public:

        explicit SecureDynamicBuffer(uint32_t size) : ser4cpp::Buffer(size)
        {}

        ~SecureDynamicBuffer()
        {
            Crypto::zero_memory(this->as_wslice());
        }

    };


}

#endif
