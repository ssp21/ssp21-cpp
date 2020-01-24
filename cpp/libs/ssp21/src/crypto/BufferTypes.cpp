
#include "ssp21/crypto/BufferTypes.h"

#include "ssp21/crypto/Crypto.h"

#include <cstring>

namespace ssp21 {

seq32_t BufferBase::as_seq() const
{
    return this->buffer.as_seq(lookup_length_in_bytes(this->length));
}

uint8_t BufferBase::get_length_in_bytes() const
{
    return lookup_length_in_bytes(this->length);
}

BufferLength BufferBase::get_length() const
{
    return this->length;
}

wseq32_t BufferBase::as_wseq()
{
    return this->buffer.as_wseq();
}

void BufferBase::set_length(BufferLength length)
{
    this->length = length;
}

void BufferBase::copy(const BufferBase& other)
{
    this->length = other.length;
    memcpy(this->buffer.as_wseq(), other.buffer.as_seq(), other.get_length_in_bytes());
}

uint8_t BufferBase::lookup_length_in_bytes(BufferLength length)
{
    switch (length) {
    case (BufferLength::length_16):
        return 16;
    case (BufferLength::length_32):
        return 32;
    case (BufferLength::length_64):
        return 64;
    default:
        return 0;
    }
}

SecureBuffer::~SecureBuffer()
{
    Crypto::zero_memory(this->buffer.as_wseq());
}

void SecureBuffer::zero()
{
    this->set_length(BufferLength::empty);
    Crypto::zero_memory(this->buffer.as_wseq());
}

}
