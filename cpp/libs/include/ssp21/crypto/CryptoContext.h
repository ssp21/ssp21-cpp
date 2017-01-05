
#ifndef SSP21_CRYPTO_CONTEXT_H
#define SSP21_CRYPTO_CONTEXT_H

#include "ssp21/crypto/Session.h"
#include "ssp21/crypto/Handshake.h"
#include "ssp21/crypto/Reassembler.h"
#include "ssp21/crypto/TxState.h"

#include "ssp21/LayerInterfaces.h"
#include "ssp21/IFrameWriter.h"

#include "openpal/executor/IExecutor.h"

namespace ssp21
{
    /**
    * Machinery shared by both the responder and the initiator
    */
    struct CryptoContext
    {
        struct Config
        {
            /// The maximum size of a reassembled message
            uint16_t max_reassembly_size = consts::link::max_config_payload_size;
        };

        CryptoContext(
            EntityType type,
            const Config& context_config,
            const Session::Config& session_config,
            const openpal::Logger& logger,
            const std::shared_ptr<IFrameWriter>& frame_writer,
            const std::shared_ptr<openpal::IExecutor>& executor,
            std::unique_ptr<KeyPair> local_static_key_pair,
            std::unique_ptr<PublicKey> remote_static_public_key
        );

		/// ------ helper methods ------

		void bind_layers(ILowerLayer& lower, IUpperLayer& upper)
		{
			this->lower = &lower;
			this->upper = &upper;			
		}

        /// ------ member variables ------

        openpal::Logger logger;
        const std::shared_ptr<IFrameWriter> frame_writer;
        const std::shared_ptr<openpal::IExecutor> executor;

        std::unique_ptr<KeyPair> local_static_key_pair;
        std::unique_ptr<PublicKey> remote_static_public_key;

        Handshake handshake;
        Session session;
        Reassembler reassembler;
        TxState tx_state;

        ILowerLayer* lower = nullptr;
		IUpperLayer* upper = nullptr;
    };
}

#endif
