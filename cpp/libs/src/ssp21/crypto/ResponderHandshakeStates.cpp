
#include "ssp21/crypto/ResponderHandshakeStates.h"

#include "ssp21/msg/ReplyHandshakeBegin.h"

#include "openpal/logging/LogMacros.h"
#include "ssp21/LogLevels.h"


namespace ssp21
{

HandshakeStateIdle HandshakeStateIdle::instance;
	
Responder::IHandshakeState& HandshakeStateIdle::on_message(Responder::Context& ctx, const openpal::RSlice& msg_bytes, const RequestHandshakeBegin& msg)
{	
	auto err = validate_handshake_begin(ctx, msg);

	if (any(err))
	{
		FORMAT_LOG_BLOCK(ctx.logger, levels::warn, "handshake error: %s", HandshakeErrorSpec::to_string(err));
		ctx.reply_with_handshake_error(err);
		return *this;
	}

	Seq8 public_ephem_dh_key(ctx.handshake.initialize());	// generate our local ephemeral keys
	ctx.handshake.set_ck(msg_bytes);						// initialize the chaining key

	// now format our response - in the future, this we'll add certificates after this call
	ReplyHandshakeBegin reply(public_ephem_dh_key);

	auto dest = ctx.tx_buffer.as_wslice();
	auto result = reply.write_msg(dest);

	if (result.is_error()) {
		FORMAT_LOG_BLOCK(ctx.logger, levels::error, "error formatting reply: %s", FormatErrorSpec::to_string(result.err));
		return *this;
	}

	std::error_code ec;

	ctx.handshake.derive_authentication_key(
		result.written,
		ctx.local_static_key_pair->private_key,
		msg.ephemeral_public_key,
		ctx.remote_static_public_key->as_slice(),
		ec
	);

	if (ec)
	{
		FORMAT_LOG_BLOCK(ctx.logger, levels::error, "error deriving auth key: %s", ec.message().c_str());
		ctx.reply_with_handshake_error(HandshakeError::internal);
		return *this;
	}

	ctx.lower->transmit(Message(Addresses(), result.written)); // begin transmitting the response

	return *this;
}

Responder::IHandshakeState& HandshakeStateIdle::on_message(Responder::Context& ctx, const openpal::RSlice& msg_bytes, const RequestHandshakeAuth& msg)
{
	return *this;
}

HandshakeError HandshakeStateIdle::validate_handshake_begin(Responder::Context& ctx, const RequestHandshakeBegin& msg)
{	
	if (msg.version != consts::crypto::protocol_version)
	{
		return HandshakeError::unsupported_version;
	}

	// verify that the public key length matches the DH mode
	if (msg.ephemeral_public_key.length() != consts::crypto::x25519_key_length)
	{
		return HandshakeError::bad_message_format;
	}

	if (msg.certificate_mode != CertificateMode::preshared_keys)
	{
		return HandshakeError::unsupported_certificate_mode;
	}

	if (msg.certificates.count() != 0)
	{
		return HandshakeError::bad_message_format;
	}

	// last thing we should do is configure the requested algorithms
	return ctx.handshake.set_algorithms(
		Algorithms::Config(
			msg.dh_mode,
			msg.hash_mode,
			msg.nonce_mode,
			msg.session_mode
		)
	);
}
	
      
}


