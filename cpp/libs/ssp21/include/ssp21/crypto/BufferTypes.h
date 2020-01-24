
#ifndef SSP21_BUFFER_TYPES_H
#define SSP21_BUFFER_TYPES_H

#include "ssp21/crypto/Constants.h"

#include "ssp21/util/SequenceTypes.h"

#include "ser4cpp/container/StaticBuffer.h"
#include "ser4cpp/util/Uncopyable.h"

namespace ssp21 {

enum class BufferLength {
    empty,
    length_16,
    length_32,
    length_64
};

class BufferBase : private ser4cpp::Uncopyable {
public:
    virtual ~BufferBase() {}

    seq32_t as_seq() const;

    wseq32_t as_wseq();

    BufferLength get_length() const;

    void set_length(BufferLength key_type);

    void copy(const BufferBase& other);

    uint8_t get_length_in_bytes() const;

    static uint8_t lookup_length_in_bytes(BufferLength);

protected:
    BufferBase() = default;

private:
    BufferLength length = BufferLength::empty;

protected:
    ser4cpp::StaticBuffer<uint32_t, consts::crypto::max_primitive_buffer_length> buffer;
};

/**
    A secure key zeros its buffer upon destruction
    and provides a clear method
*/
class SecureBuffer : public BufferBase {
public:
    virtual ~SecureBuffer();

    void zero();

protected:
    SecureBuffer() {}
};

// specialized types that actually get used

class PublicKey final : public BufferBase {
public:
    PublicKey() {}

    PublicKey(const PublicKey& other)
    {
        this->copy(other);
    }
};

class PrivateKey final : public SecureBuffer {
public:
    PrivateKey() {}

    PrivateKey(const PrivateKey& other)
    {
        this->copy(other);
    }
};

class DHOutput final : public SecureBuffer {
};

// either an HMAC or an AEAD tag
class MACOutput final : public SecureBuffer {
};

class HashOutput : public SecureBuffer {
};

class DSAOutput final : public SecureBuffer {
};

class SymmetricKey final : public SecureBuffer {
};

struct KeyPair final {
    PublicKey public_key;
    PrivateKey private_key;
};

struct SessionKeys final {
    SymmetricKey rx_key;
    SymmetricKey tx_key;

    inline void zero()
    {
        this->rx_key.zero();
        this->tx_key.zero();
    }

    inline void copy(const SessionKeys& other)
    {
        this->rx_key.copy(other.rx_key);
        this->tx_key.copy(other.tx_key);
    }

    inline bool valid() const
    {
        return (rx_key.get_length() == BufferLength::length_32) && (tx_key.get_length() == BufferLength::length_32);
    }
};

}

#endif
