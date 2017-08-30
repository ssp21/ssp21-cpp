
#ifndef SSP21_ISESSIONMODE_H
#define SSP21_ISESSIONMODE_H


#include "ssp21/util/SequenceTypes.h"
#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/gen/SessionData.h"
#include "ssp21/IFrameWriter.h"

#include "ssp21/crypto/gen/CryptoError.h"

#include <system_error>

namespace ssp21
{

    class ISessionMode
    {

    public:

        virtual ~ISessionMode() {}

        // checks preconditions and invokes the read implementation
        seq32_t read(const SymmetricKey& key, const SessionData& msg, wseq32_t dest, std::error_code& ec) const
        {
            if (key.get_type() != BufferType::symmetric_key)
            {
                ec = CryptoError::bad_buffer_size;
                return seq32_t::empty();
            }

            return this->read_impl(key, msg, dest, ec);
        }

        // checks preconditions and invokes the write implementation
        seq32_t write(IFrameWriter& writer, const SymmetricKey& key, const AuthMetadata& metadata, seq32_t& user_data, std::error_code& ec) const
        {
            if (key.get_type() != BufferType::symmetric_key)
            {
                ec = CryptoError::bad_buffer_size;
                return seq32_t::empty();
            }

            return this->write_impl(writer, key, metadata, user_data, ec);
        }

    protected:

        /**
        * Authenticates (and possibly decrypts) a session message payload and returns a slice pointing to the cleartext output.
        *
        * @key the symmetric key used for authentication (and optionally decryption)
        * @msg the parsed session data message
        * @dest The output buffer into which the cleartext may be written (encryption modes only) if no error occurs.
        * @ec An error condition will be signaled if the output buffer is too small or if an authentication error occurs
        *
        * @return A slice pointing to the cleartext. This slice will be empty if an error occured.
        */
        virtual seq32_t read_impl(
            const SymmetricKey& key,
            const SessionData& msg,
            wseq32_t dest,
            std::error_code& ec
        ) const = 0;

        /**
        * Writes a SessionData message using the supplied IFrameWriter
        *
        * @writer interface used to write the message (and any framing) to an output buffer owned by the IFrameWriter
        * @key the symmetric key used for authentication (and possibly encryption)
        * @metadata the metadata to use with the message
        * @user_data the cleartext userdata that will be authenticated (and possibly encrypted). Buffer mat only be partially consumed if insufficient space.
        * @ec An error condition will be signaled if the output buffer is too small for the payload
        *
        * @return A slice pointing to the possibly encrypted user data. This slice will be empty if an error occured.
        */
        virtual seq32_t write_impl(
            IFrameWriter& writer,
            const SymmetricKey& key,
			const AuthMetadata& metadata,
            seq32_t& user_data,
            std::error_code& ec
        ) const = 0;

        typedef openpal::StaticBuffer<uint32_t, AuthMetadata::fixed_size_bytes> metadata_buffer_t;
        typedef openpal::StaticBuffer<uint32_t, openpal::UInt16::size> user_data_length_buffer_t;

        inline static seq32_t get_metadata_bytes(const AuthMetadata& metadata, metadata_buffer_t& buffer)
        {
            auto dest = buffer.as_wseq();
            metadata.write(dest);
            return buffer.as_seq();
        }

        inline static seq32_t get_user_data_length_bytes(uint16_t user_data_length, user_data_length_buffer_t& buffer)
        {
            auto dest = buffer.as_wseq();
            openpal::UInt16::write_to(dest, user_data_length);
            return buffer.as_seq();
        }

    };

}

#endif
