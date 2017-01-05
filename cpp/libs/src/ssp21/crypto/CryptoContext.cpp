
#include "ssp21/crypto/CryptoContext.h"

namespace ssp21
{

    CryptoContext::CryptoContext(
        EntityType type,
        const openpal::Logger& logger,
        const std::shared_ptr<IFrameWriter>& frame_writer,
        const std::shared_ptr<openpal::IExecutor>& executor,
        const Session::Config& session_config,
        const Config& context_config,
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


}


