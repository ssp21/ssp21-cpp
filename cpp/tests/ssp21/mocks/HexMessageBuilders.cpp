
#include "HexMessageBuilders.h"

#include "openpal/container/StaticBuffer.h"

#include "testlib/HexConversions.h"

#include "ssp21/crypto/gen/RequestHandshakeBegin.h"
#include "ssp21/crypto/gen/ReplyHandshakeBegin.h"
#include "ssp21/crypto/gen/ReplyHandshakeError.h"
#include "ssp21/crypto/gen/SessionData.h"

#include "ssp21/link/LinkFrameWriter.h"

#include "HexSequences.h"

#include <vector>
#include <assert.h>

using namespace openpal;

namespace ssp21
{
    namespace hex
    {

        std::string write_message(const IMessage& msg)
        {
            openpal::StaticBuffer<uint32_t, 1024> buffer;
            auto dest = buffer.as_wseq();
            auto result = msg.write(dest);

            assert(!result.is_error());

            return to_hex(result.written);
        }

        std::string repeat(uint8_t value, uint8_t count)
        {
            return openpal::repeat_hex(value, count);
        }

        std::string func_to_hex(Function func)
        {
            return byte_to_hex(FunctionSpec::to_type(func));
        }

        std::string request_handshake_begin(
            uint16_t version,
            NonceMode nonce_mode,
            HandshakeEphemeral handshake_ephemeral,
            HandshakeHash handshake_hash,
            HandshakeKDF handshake_kdf,
            SessionMode session_mode,
            uint16_t max_nonce,
            uint32_t max_session_time,
            CertificateMode certificate_mode,
            const std::string& hex_ephem_pub_key
        )
        {
            HexSeq pub_key(hex_ephem_pub_key);

            RequestHandshakeBegin msg(
                version,
                CryptoSpec(
                    nonce_mode,
                    handshake_ephemeral,
                    handshake_hash,
                    handshake_kdf,
                    session_mode
                ),
                SessionConstraints(
                    max_nonce,
                    max_session_time
                ),
                certificate_mode,
                pub_key,
                seq32_t::empty()
            );

            return write_message(msg);
        }

        std::string reply_handshake_begin(
            const std::string& hex_ephem_pub_key
        )
        {
            HexSeq pub_key(hex_ephem_pub_key);

            ReplyHandshakeBegin msg(pub_key, seq32_t::empty());

            return write_message(msg);
        }

        std::string reply_handshake_error(HandshakeError err)
        {
            ReplyHandshakeError msg(err);
            return write_message(msg);
        }

        std::string session_data(uint16_t nonce, uint32_t valid_until, const std::string& user_data, const std::string& auth_tag)
        {
            HexSeq user_data_hex(user_data);
            HexSeq auth_tag_hex(auth_tag);

            SessionData msg(
                AuthMetadata(
                    nonce,
                    valid_until
                ),
                user_data_hex,
                auth_tag_hex
            );

            return write_message(msg);
        }

        std::string session_auth(uint32_t valid_until, const std::string& user_data, const std::string& auth_tag)
        {
            return session_data(0, valid_until, user_data, auth_tag);
        }

        class SeqWritable : public IWritable
        {

            seq32_t payload;

        public:

            SeqWritable(seq32_t payload) : payload(payload)
            {}

            virtual FormatResult write(wseq32_t& output) const
            {
                const auto result = output.copy_from(payload);
                return result.empty() ? FormatResult::error(FormatError::insufficient_space) : FormatResult::success(result);
            }

            virtual void print(IMessagePrinter& printer) const {}

            virtual Function get_function() const
            {
                return Function::undefined;
            }
        };

        std::string link_frame(uint16_t src, uint16_t dest, const std::string& payload)
        {
            const auto max = consts::link::max_config_payload_size;

            Hex data(payload);
            if (data.as_rslice().length() > max) throw std::logic_error("payload length exceeds maximum");
            const auto payload_u16 = data.as_rslice().take(max);

            LinkFrameWriter writer(openpal::Logger::empty(), Addresses(dest, src), max);

            const auto result = writer.write(SeqWritable(payload_u16));

            if (result.is_error()) throw std::logic_error("error writing frame");

            return to_hex(result.frame);
        }

    }
}

