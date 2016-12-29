
#include "HexMessageBuilders.h"

#include "openpal/container/StaticBuffer.h"

#include "testlib/HexConversions.h"

#include "ssp21/crypto/gen/RequestHandshakeBegin.h"
#include "ssp21/crypto/gen/ReplyHandshakeBegin.h"
#include "ssp21/crypto/gen/RequestHandshakeAuth.h"
#include "ssp21/crypto/gen/ReplyHandshakeAuth.h"
#include "ssp21/crypto/gen/ReplyHandshakeError.h"
#include "ssp21/crypto/gen/SessionData.h"

#include "HexSequences.h"

#include "MakeUnique.h"

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
            auto dest = buffer.as_wslice();
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
            DHMode dh_mode,
            HandshakeHash handshake_hash,
            HandshakeKDF handshake_kdf,
            HandshakeMAC handshake_mac,
            SessionMode session_mode,
            CertificateMode certificate_mode,
            const std::string& hex_ephem_pub_key,
            std::initializer_list<std::string> certificates
        )
        {
            HexSeq8 pub_key(hex_ephem_pub_key);

            RequestHandshakeBegin msg(
                version,
                CryptoSpec(
                    nonce_mode,
                    dh_mode,
                    handshake_hash,
                    handshake_kdf,
                    handshake_mac,
                    session_mode
                ),
                certificate_mode,
                pub_key
            );

            std::vector<std::unique_ptr<ssp21::HexSeq16>> certificate_slices;

            for (auto& cert : certificates)
            {
                auto hex = std::make_unique<ssp21::HexSeq16>(cert);
                msg.certificates.push(hex->to_seq());
                certificate_slices.push_back(std::move(hex));
            }

            return write_message(msg);
        }

        std::string request_handshake_auth(const std::string& mac)
        {
            HexSeq8 mac_bytes(mac);
            RequestHandshakeAuth request(mac_bytes);
            return write_message(request);
        }

        std::string reply_handshake_auth(const std::string& mac)
        {
            HexSeq8 mac_bytes(mac);
            ReplyHandshakeAuth reply(mac_bytes);
            return write_message(reply);
        }

        std::string reply_handshake_begin(
            const std::string& hex_ephem_pub_key,
            std::initializer_list<std::string> certificates
        )
        {
            HexSeq8 pub_key(hex_ephem_pub_key);

            ReplyHandshakeBegin msg(pub_key);

            std::vector<std::unique_ptr<HexSeq16>> certificate_slices;

            for (auto& cert : certificates)
            {
                auto hex = std::make_unique<HexSeq16>(cert);
                msg.certificates.push(*hex);
                certificate_slices.push_back(std::move(hex));
            }

            return write_message(msg);
        }

        std::string reply_handshake_error(HandshakeError err)
        {
            ReplyHandshakeError msg(err);
            return write_message(msg);
        }

        std::string session_data(uint16_t nonce, uint32_t valid_until, bool fir, bool fin, const std::string& user_data, const std::string& auth_tag)
        {
            HexSeq16 user_data_hex(user_data);
            HexSeq8 auth_tag_hex(auth_tag);

            SessionData msg(
                AuthMetadata(
                    nonce,
                    valid_until,
                    SessionFlags(fir, fin)
                ),
                user_data_hex,
                auth_tag_hex
            );

            return write_message(msg);
        }

    }
}

