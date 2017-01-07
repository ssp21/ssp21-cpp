
#include "ssp21/crypto/InitiatorHandshakeStates.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/LogLevels.h"

namespace ssp21
{

    Initiator::IHandshakeState* InitiatorHandshakeIdle::on_handshake_required(Initiator& ctx, const openpal::Timestamp& now)
    {
		const CryptoSpec crypto_spec(
			ctx.suite.nonce_mode,
			ctx.suite.dh_mode,
			ctx.suite.handshake_hash,
			ctx.suite.handshake_kdf,
			ctx.suite.handshake_mac,
			ctx.suite.session_mode
		);

		const auto err = ctx.handshake.set_algorithms(crypto_spec);
		if (any(err))
		{
			FORMAT_LOG_BLOCK(ctx.logger, levels::error, "Error configured handshake algorithms: %s", HandshakeErrorSpec::to_string(err));

			// TODO  -  set retry timer or instead transition to a failed state?
			return InitiatorHandshakeWaitForRetry::get();
		}
				
		const auto public_key = ctx.handshake.initialize();		

		RequestHandshakeBegin request(
			consts::crypto::protocol_version,
			crypto_spec,
			CertificateMode::preshared_keys,
			public_key
		);

		const auto result = ctx.frame_writer->write(request);

		if (result.is_error())
		{
			// TODO  -  set retry timer or instead transition to a failed state?
			return InitiatorHandshakeWaitForRetry::get();
		}

		ctx.handshake.set_ck(result.written);

		ctx.lower->transmit(result.frame);

        return InitiatorHandshakeWaitForBeginReply::get();
    }
}


