
#include "ssp21/crypto/Initiator.h"

namespace ssp21
{


    Initiator::Initiator(
        const Config& context_config,
        const Session::Config& session_config,
        const openpal::Logger& logger,
        const std::shared_ptr<IFrameWriter>& frame_writer,
        const std::shared_ptr<openpal::IExecutor>& executor,
        std::unique_ptr<KeyPair> local_static_key_pair,
        std::unique_ptr<PublicKey> remote_static_public_key
    ) :
        CryptoLayer(
            HandshakeMode::Initiator,
            context_config,
            session_config,
            logger,
            frame_writer,
            executor,
            std::move(local_static_key_pair),
            std::move(remote_static_public_key)
        )
    {}

    void Initiator::reset_state_on_close()
    {}

    bool Initiator::supports(Function function) const
    {
        switch (function)
        {
        case(Function::reply_handshake_begin):
        case(Function::reply_handshake_auth):
        case(Function::reply_handshake_error):
        case(Function::session_data):
            return true;
        default:
            return false;
        }
    }

    bool Initiator::on_message(const ReplyHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        return false;
    }

    bool Initiator::on_message(const ReplyHandshakeAuth& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        return false;
    }

    bool Initiator::on_message(const ReplyHandshakeError& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        return false;
    }

}


