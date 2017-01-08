
#include "ssp21/crypto/InitiatorHandshakeStates.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/LogLevels.h"

namespace ssp21
{

    Initiator::IHandshakeState* InitiatorHandshakeIdle::on_handshake_required(Initiator& ctx, const openpal::Timestamp& now)
    {
        if (!ctx.lower->get_is_tx_ready())
        {
            return this;
        }

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
            // There's nothing we can do about a bad configuration, so just go to an error state
            return InitiatorHandshakeBadConfiguration::get();
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
            return InitiatorHandshakeBadConfiguration::get();
        }

        ctx.handshake.set_ck(result.written);

        ctx.lower->transmit(result.frame);

        ctx.start_response_timer();

        return InitiatorHandshakeWaitForBeginReply::get();
    }
}


