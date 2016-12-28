
#ifndef SSP21_ISESSIONMODE_H
#define SSP21_ISESSIONMODE_H

#include "openpal/container/RSlice.h"
#include "openpal/container/WSlice.h"

#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/gen/SessionData.h"

#include <system_error>

namespace ssp21
{
    typedef openpal::StaticBuffer<AuthMetadata::fixed_size_bytes> metadata_buffer_t;

    class ISessionMode
    {

    public:

        virtual ~ISessionMode() {}

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
        virtual openpal::RSlice read(
            const SymmetricKey& key,
            const SessionData& msg,
            openpal::WSlice dest,
            std::error_code& ec
        ) const = 0;

        /**
        * Writes an authenticated (and possibly encrypted) payload into the destination output buffer
        *
        * @key the symmetric key used for authentication (and possibly encryption)
        * @metadata associated data that is also covered by the authentication tag
        * @userdata the cleartext userdata that will be authentiacted (and possibly encrypted) and placed placed into the destination buffer
        * @auth_tag buffer into which the authentication tag will be written. No modification if an error occured.
        * @dest The output buffer into which the encrypted payload will be written if this mode is an encryption mode
        * @ec An error condition will be signaled if the output buffer is too small for the payload
        *
        * @return A slice pointing to the possibly encrypted user data. This slice will be empty if an error occured.
        */
        virtual openpal::RSlice write(
            const SymmetricKey& key,
            const AuthMetadata& metadata,
            const openpal::RSlice& userdata,
            AuthenticationTag& auth_tag,
            openpal::WSlice dest,
            std::error_code& ec
        ) const = 0;

        /**
        * Calculates the maximum possible user data length factoring the overhead from any authentication tags and padding
        *
        * @max_payload_size the maximum allowed payload size
        *
        * @return maximum possible user data length
        */
        virtual uint16_t max_writable_user_data_length(uint16_t max_payload_size) const = 0;

    protected:

        typedef openpal::StaticBuffer<AuthMetadata::fixed_size_bytes> metadata_buffer_t;

        inline static openpal::RSlice get_metadata_bytes(const AuthMetadata& metadata, metadata_buffer_t& buffer)
        {
            auto dest = buffer.as_wslice();
            metadata.write(dest);
            return buffer.as_rslice();
        }

    };

}

#endif
