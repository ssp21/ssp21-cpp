
#include "ssp21/crypto/BufferTypes.h"

#include "ssp21/crypto/Crypto.h"

#include <cstring>

namespace ssp21
{
    seq8_t BufferBase::as_seq() const
    {
        return this->buffer.as_seq(get_buffer_length(this->buffer_type));
    }

    BufferType BufferBase::get_type() const
    {
        return this->buffer_type;
    }

    wseq32_t BufferBase::as_wseq()
    {
        return this->buffer.as_wseq();
    }

    void BufferBase::set_type(BufferType buffer_type)
    {
        this->buffer_type = buffer_type;
        this->length = get_buffer_length(buffer_type);
    }

    void BufferBase::copy(const BufferBase& other)
    {
		this->buffer_type = other.buffer_type;
        memcpy(this->buffer.as_wseq(), other.buffer.as_seq(), get_buffer_length(other.buffer_type));
    }

    uint8_t BufferBase::get_buffer_length(BufferType key_type)
    {
        switch (key_type)
        {
        case(BufferType::x25519_key) :
            return consts::crypto::x25519_key_length;
        case(BufferType::sha256) :
            return consts::crypto::sha256_hash_output_length;
        case(BufferType::symmetric_key) :
            return consts::crypto::symmetric_ley_length;
        default:
            return 0;
        }
    }

    SecureBuffer::~SecureBuffer()
    {
        Crypto::zero_memory(this->buffer.as_wseq());
    }

}

