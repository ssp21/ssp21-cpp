
#include "HexMessageBuilders.h"

#include "ser4cpp/container/StaticBuffer.h"
#include "ser4cpp/util/HexConversions.h"

#include "crypto/gen/ReplyHandshakeBegin.h"
#include "crypto/gen/ReplyHandshakeError.h"
#include "crypto/gen/RequestHandshakeBegin.h"
#include "crypto/gen/SessionData.h"

#include "link/LinkFrameWriter.h"

#include "HexSequences.h"

#include <assert.h>
#include <vector>

namespace ssp21 {
namespace hex {

    std::string write_message(const IMessage& msg)
    {
        ser4cpp::StaticBuffer<uint32_t, 1024> buffer;
        auto dest = buffer.as_wseq();
        auto result = msg.write(dest);

        assert(!result.is_error());

        return ser4cpp::HexConversions::to_hex(result.written);
    }

    std::string repeat(uint8_t value, uint8_t count)
    {
        return ser4cpp::HexConversions::repeat_hex(value, count);
    }

    std::string func_to_hex(Function func)
    {
        return ser4cpp::HexConversions::byte_to_hex(FunctionSpec::to_type(func));
    }

    std::string request_handshake_begin(
        uint16_t version,
        SessionNonceMode nonce_mode,
        HandshakeEphemeral handshake_ephemeral,
        HandshakeHash handshake_hash,
        HandshakeKDF handshake_kdf,
        SessionCryptoMode session_mode,
        uint16_t max_nonce,
        uint32_t max_session_time,
        HandshakeMode handshake_mode,
        const std::string& hex_ephem_pub_key)
    {
        HexSeq pub_key(hex_ephem_pub_key);

        RequestHandshakeBegin msg(
            version,
            CryptoSpec(
                handshake_ephemeral,
                handshake_hash,
                handshake_kdf,
                nonce_mode,
                session_mode),
            SessionConstraints(
                max_nonce,
                max_session_time),
            handshake_mode,
            pub_key,
            seq32_t::empty());

        return write_message(msg);
    }

    std::string reply_handshake_begin(
        const std::string& hex_ephem_pub_key)
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
                valid_until),
            user_data_hex,
            auth_tag_hex);

        return write_message(msg);
    }

    std::string session_auth(uint32_t valid_until, const std::string& user_data, const std::string& auth_tag)
    {
        return session_data(0, valid_until, user_data, auth_tag);
    }

    class SeqWritable : public IWritable {

        seq32_t payload;

    public:
        SeqWritable(seq32_t payload)
            : payload(payload)
        {
        }

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

        const auto data = ser4cpp::HexConversions::from_hex(payload);
        if (data->as_rslice().length() > max)
            throw std::logic_error("payload length exceeds maximum");
        const auto payload_u16 = data->as_rslice().take(max);

        LinkFrameWriter writer(log4cpp::Logger::empty(), Addresses(dest, src), max);

        const auto result = writer.write(SeqWritable(payload_u16));

        if (result.is_error())
            throw std::logic_error("error writing frame");

        return ser4cpp::HexConversions::to_hex(result.frame);
    }

}
}
