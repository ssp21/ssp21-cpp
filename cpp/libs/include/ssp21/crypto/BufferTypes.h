
#ifndef SSP21_BUFFER_TYPES_H
#define SSP21_BUFFER_TYPES_H

#include "Constants.h"

#include <openpal/util/Uncopyable.h>
#include <openpal/container/StaticBuffer.h>

namespace ssp21
{
// the buffer_ type also defines the length
    enum class BufferType
    {
        empty,
        x25519_key,
        sha256,
        symmetric_key
    };

    class BufferBase : private openpal::Uncopyable
    {
    public:

        virtual ~BufferBase() {}

        openpal::RSlice as_slice() const;

        BufferType get_type() const;

        openpal::WSlice get_write_slice();

        void set_type(BufferType key_type);

    protected:

        BufferBase();

    private:

        static uint32_t get_buffer_length(BufferType);

        uint32_t length_;
        BufferType buffer_type_;

    protected:
        openpal::StaticBuffer<consts::max_primitive_buffer_length> buffer_;
    };

    /**
    	A secure key zeros its buffer upon destruction
    	and provides a clear method
    */
    class SecureBuffer : public BufferBase
    {
    public:
        virtual ~SecureBuffer();

    protected:
        SecureBuffer() {}
    };

// specialized types that actually get used

    class PublicKey final : public BufferBase {};

    class PrivateKey final : public SecureBuffer {};

    class DHOutput final : public SecureBuffer {};

    class HashOutput final : public SecureBuffer {};

    class SymmetricKey final : public SecureBuffer {};

    struct KeyPair final
    {
        PublicKey public_key;
        PrivateKey private_key;
    };
}

#endif
