
#include "ssp21/crypto/CryptoLayer.h"

#include "ssp21/crypto/LogMessagePrinter.h"
#include "ssp21/LogLevels.h"

#include "openpal/logging/LogMacros.h"


namespace ssp21
{

    CryptoLayer::CryptoLayer(
        EntityType type,
        const Config& context_config,
        const Session::Config& session_config,
        const openpal::Logger& logger,
        const std::shared_ptr<IFrameWriter>& frame_writer,
        const std::shared_ptr<openpal::IExecutor>& executor,
        std::unique_ptr<KeyPair> local_static_key_pair,
        std::unique_ptr<PublicKey> remote_static_public_key
    ) :
        logger(logger),
        frame_writer(frame_writer),
        executor(executor),
        local_static_key_pair(std::move(local_static_key_pair)),
        remote_static_public_key(std::move(remote_static_public_key)),
        handshake(type),
        session(frame_writer, session_config),
        reassembler(context_config.max_reassembly_size)
    {}

    template <class MsgType>
    bool CryptoLayer::handle_message(const seq32_t& message, const openpal::Timestamp& now)
    {
        MsgType msg;
        auto err = msg.read(message);
        if (any(err))
        {
            FORMAT_LOG_BLOCK(logger, levels::warn, "Error parsing message (%s): %s", FunctionSpec::to_string(MsgType::function), ParseErrorSpec::to_string(err));
            this->on_parse_error(MsgType::function, err);
            return false;
        }
        else
        {
            FORMAT_LOG_BLOCK(this->logger, levels::rx_crypto_msg, "%s", FunctionSpec::to_string(MsgType::function));

            if (this->logger.is_enabled(levels::rx_crypto_msg_fields))
            {
                LogMessagePrinter printer(this->logger, levels::rx_crypto_msg_fields);
                msg.print(printer);
            }

            this->on_message(msg, message, now);

            return true;
        }
    }

	bool CryptoLayer::on_rx_ready_impl(const seq32_t& data)
	{
		if (this->can_receive())
		{
			this->process(data);

			return true;
		}
		else
		{
			return false;
		}
	}

	void CryptoLayer::on_close_impl()
	{
		// let the super class reset
		this->reset_state();

		this->session.reset();
		this->reassembler.reset();
		this->upper->on_close();
		this->tx_state.reset();
		this->reset_lower_layer();		
	}

	void CryptoLayer::receive()
	{
		if (this->is_rx_ready && this->upper->on_rx_ready(this->reassembler.get_data()))
		{
			this->is_rx_ready = false;
		}
	}

    bool CryptoLayer::process(const seq32_t& message)
    {
        const auto now = this->executor->get_time();

        if (message.is_empty())
        {
            SIMPLE_LOG_BLOCK(logger, levels::warn, "Received zero length message");
            return false;
        }

        const auto raw_function = message[0];
        const auto function = FunctionSpec::from_type(raw_function);

        if (!this->supports(function))
        {
            FORMAT_LOG_BLOCK(logger, levels::warn, "Received unsupported function: %s(%u)", FunctionSpec::to_string(function), raw_function);
            return false;
        }

        switch (function)
        {
        case(Function::request_handshake_begin):
            return handle_message<RequestHandshakeBegin>(message, now);
        case(Function::request_handshake_auth):
            return handle_message<RequestHandshakeAuth>(message, now);
        case(Function::reply_handshake_begin):
            return handle_message<ReplyHandshakeBegin>(message, now);
        case(Function::reply_handshake_auth):
            return handle_message<ReplyHandshakeAuth>(message, now);
        case(Function::reply_handshake_error):
            return handle_message<ReplyHandshakeError>(message, now);
        case(Function::session_data):
            return handle_message<SessionData>(message, now);
        default:
            return false;
        }
    }

	void CryptoLayer::check_receive()
	{
		if (this->can_receive())
		{
			this->lower->receive();
		}
	}


}


