
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
        return Result::failure(HandshakeError::internal);
    }

}

