
#ifndef SSP21_ISESSIONMODE_H
#define SSP21_ISESSIONMODE_H

#include "IFrameWriter.h"
#include "crypto/gen/SessionData.h"
#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/util/SequenceTypes.h"

#include "ssp21/crypto/gen/CryptoError.h"

#include <system_error>

namespace ssp21 {

class ISessionMode {

public:
    virtual ~ISessionMode() {}

    // checks preconditions and invokes the read implementation
    seq32_t read(const SymmetricKey& key, const SessionData& msg, wseq32_t dest, std::error_code& ec) const
    {
        if (key.get_type() != BufferType::symmetric_key) {
            ec = CryptoError::bad_buffer_size;
            return seq32_t::empty();
        }

        return this->read_impl(key, msg, dest, ec);
    }

    // checks preconditions and invokes the write implementation
    SessionData write(const SymmetricKey& key, const AuthMetadata& metadata, seq32_t& user_data, wseq32_t encrypt_buffer, MACOutput& mac, std::error_code& ec) const
    {
        if (key.get_type() != BufferType::symmetric_key) {
            ec = CryptoError::bad_buffer_size;
            return SessionData();
        }

        return this->write_impl(key, metadata, user_data, encrypt_buffer, mac, ec);
    }

protected:
    /**
        * Authenticates (and possibly decrypts) a session message payload and returns a slice pointing to the cleartext output.
        *
        * @key the symmetric key used for authentication (and optionally decryption)
        * @msg the parsed session data message
        * @dest The output buffer into which the authenticated data is written if no error occurs.
        * @ec An error condition will be signaled if the output buffer is too small or if an authentication error occurs
        *
        * @return A slice pointing to the cleartext. This slice will be empty if an error occured.
        */
    virtual seq32_t read_impl(
        const SymmetricKey& key,
        const SessionData& msg,
        wseq32_t dest,
        std::error_code& ec) const = 0;

    /**
        * Creates a SessionData struct ready for transmisson. The user_data field may point to an encrypted payload or to a segment of user_data if
		* the implementation is auth-only.
        *
        * @key the symmetric key used for authentication (and possibly encryption)
        * @metadata the metadata to use with the message
        * @user_data the cleartext userdata that will be authenticated (and possibly encrypted). Buffer mat only be partially consumed if insufficient space.
		* @encrypt_buffer an output buffer where the implementation may place the encrypted part of the AEAD.
		* @mac buffer where the MAC (HMAC or detached AEAD tag) will be placed
        * @ec An error condition will be signaled if encrypt_buffer is too small for the payload
        *
        * @return A SessionData struct ready for transmission, or an error code
        */
    virtual SessionData write_impl(const SymmetricKey& key, const AuthMetadata& metadata, seq32_t& user_data, wseq32_t encrypt_buffer, MACOutput& mac, std::error_code& ec) const = 0;

    using metadata_buffer_t = ser4cpp::StaticBuffer<uint32_t, AuthMetadata::fixed_size_bytes>;
    using user_data_length_buffer_t = ser4cpp::StaticBuffer<uint32_t, ser4cpp::UInt16::size>;

    inline static seq32_t get_metadata_bytes(const AuthMetadata& metadata, metadata_buffer_t& buffer)
    {
        auto dest = buffer.as_wseq();
        metadata.write(dest);
        return buffer.as_seq();
    }

    inline static seq32_t get_user_data_length_bytes(uint16_t user_data_length, user_data_length_buffer_t& buffer)
    {
        auto dest = buffer.as_wseq();
        ser4cpp::UInt16::write_to(dest, user_data_length);
        return buffer.as_seq();
    }
};

}

#endif
