
#include "ssp21/crypto/BufferTypes.h"

#include "ssp21/crypto/Crypto.h"

namespace ssp21
{
    openpal::RSlice BufferBase::as_slice() const
    {
        return this->buffer.as_rslice().take(this->length);
    }

    BufferType BufferBase::get_type() const
    {
        return this->buffer_type;
    }

    openpal::WSlice BufferBase::get_write_slice()
    {
        return this->buffer.as_wslice();
    }

    void BufferBase::set_type(BufferType buffer_type)
    {
        this->buffer_type = buffer_type;
        this->length = get_buffer_length(buffer_type);
    }

    void BufferBase::copy(const BufferBase& other)
    {
        memcpy(this->buffer.as_wslice(), other.buffer.as_rslice(), consts::crypto::max_primitive_buffer_length);
    }

    uint32_t BufferBase::get_buffer_length(BufferType key_type)
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
        Crypto::zero_memory(this->buffer.as_wslice());
    }

}

