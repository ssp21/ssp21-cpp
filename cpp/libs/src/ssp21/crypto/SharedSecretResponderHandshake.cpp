
#include "ssp21/crypto/SharedSecretResponderHandshake.h"

#include "ssp21/util/Exception.h"

namespace ssp21
{

    SharedSecretResponderHandshake::SharedSecretResponderHandshake(const openpal::Logger& logger, const std::shared_ptr<SymmetricKey>& shared_secret) :
        logger(logger),
        shared_secret(shared_secret)
    {
        if (shared_secret->as_seq().length() != consts::crypto::symmetric_ley_length)
        {
            throw Exception("Invalid length for shared secret: ", shared_secret->as_seq().length());
        }
    }

    IResponderHandshake::Result SharedSecretResponderHandshake::process(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now, IFrameWriter& writer, Session& session)
    {
        /*
        if(msg.spec.dh_mode)

        // verify that the public key length matches the DH mode
        if (msg.ephemeral_data.length() != consts::crypto::symmetric_ley_length)
        {
        	return Result::failure(HandshakeError::bad_message_format);
        }

        {
        	const auto err = this->cert_handler->validate(msg.certificate_mode, msg.handshake_data, remote_public_static_key);
        	if (any(err))
        	{
        		return Result::failure(err);
        	}
        }

        Algorithms algorithms;

        {
        	const auto err = algorithms.configure(msg.spec);
        	if (any(err))
        	{
        		return Result::failure(err);
        	}
        }

        // generate an ephemeral key pair
        KeyPair ephemeralKeys;
        algorithms.handshake.gen_keypair(ephemeralKeys);

        // prepare the response
        const ReplyHandshakeBegin reply(
        	ephemeralKeys.public_key.as_seq(),
        	this->cert_handler->certificate_data()
        );

        const auto result = writer.write(reply);
        if (any(result.err))
        {
        	FORMAT_LOG_BLOCK(this->logger, levels::error, "Error writing handshake reply: %s", FormatErrorSpec::to_string(result.err));
        	return Result::failure(HandshakeError::internal);
        }

        HandshakeHasher hasher;
        const auto handshake_hash = hasher.compute(algorithms.handshake.hash, msg_bytes, result.written);

        // compute the input_key_material
        TripleDH triple_dh;
        std::error_code ec;
        const auto ikm = triple_dh.compute(
        	algorithms.handshake.dh,
        	this->static_keys,
        	ephemeralKeys,
        	remote_public_static_key,
        	msg.ephemeral_data,
        	ec
        );

        if (ec)
        {
        	FORMAT_LOG_BLOCK(this->logger, levels::error, "Error calculating input key material: %s", ec.message().c_str());
        	return Result::failure(HandshakeError::internal); // TODO
        }

        SessionKeys session_keys;

        algorithms.handshake.kdf(
        	handshake_hash,
        	{ ikm.dh1, ikm.dh2, ikm.dh3 },
        	session_keys.rx_key,
        	session_keys.tx_key
        );

        session.initialize(
        	algorithms.session,
        	Session::Param(
        		now,
        		msg.constraints.max_nonce,
        		msg.constraints.max_session_duration
        	),
        	session_keys
        );
        */

        return Result::failure(HandshakeError::internal);
    }

}

