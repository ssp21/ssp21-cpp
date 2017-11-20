
#include "ssp21/stack/Factory.h"

#include "ssp21/stack/LinkCryptoStack.h"

#include "ssp21/crypto/ResponderHandshakes.h"
#include "ssp21/crypto/InitiatorHandshakes.h"

namespace ssp21
{
    namespace responder
    {

        namespace factory
        {
            std::shared_ptr<IStack> shared_secret_mode(
                const Addresses& addresses,
                const ResponderConfig& config,
                const openpal::Logger& logger,
                const std::shared_ptr<openpal::IExecutor>& executor,
                const std::shared_ptr<IKeyLookup>& key_lookup
            )
            {
                return std::make_shared<ResponderStack>(
                           addresses,
                           config,
                           logger,
                           executor,
                           ResponderHandshakes::shared_secret_mode(
                               logger,
                               key_lookup
                           )
                       );
            }

            std::shared_ptr<IStack> preshared_public_key_mode(
                const Addresses& addresses,
                const ResponderConfig& config,
                const openpal::Logger& logger,
                const std::shared_ptr<openpal::IExecutor>& executor,
                const StaticKeys& local_keys,
                const std::shared_ptr<const PublicKey>& remote_public_key
            )
            {
                return std::make_shared<ResponderStack>(
                           addresses,
                           config,
                           logger,
                           executor,
                           ResponderHandshakes::public_key_mode(
                               logger,
                               local_keys,
                               ICertificateHandler::preshared_key(remote_public_key)
                           )
                       );
            }

            std::shared_ptr<IStack> certificate_public_key_mode(
                const Addresses& addresses,
                const ResponderConfig& config,
                const openpal::Logger& logger,
                const std::shared_ptr<openpal::IExecutor>& executor,
                const StaticKeys& local_keys,
                const std::shared_ptr<ssp21::SecureDynamicBuffer>& anchor_cert_file_data,
                const std::shared_ptr<ssp21::SecureDynamicBuffer>& presented_chain_file_data
            )
            {
                return std::make_shared<ResponderStack>(
                           addresses,
                           config,
                           logger,
                           executor,
                           ResponderHandshakes::public_key_mode(
                               logger,
                               local_keys,
                               ICertificateHandler::certificates(
                                   anchor_cert_file_data,
                                   presented_chain_file_data
                               )
                           )
                       );
            }

        }
    }

    namespace initiator
    {
        namespace factory
        {
            std::shared_ptr<IStack> shared_secert_mode(
                const Addresses& addresses,
                const InitiatorConfig& config,
                const openpal::Logger& logger,
                const std::shared_ptr<openpal::IExecutor>& executor,
                const CryptoSuite& crypto_suite,
                const std::shared_ptr<IKeySource>& key_source
            )
            {
                return std::make_shared<InitiatorStack>(
                           addresses,
                           config,
                           logger,
                           executor,
                           InitiatorHandshakes::shared_secret_mode(
                               logger,
                               crypto_suite,
                               key_source
                           )
                       );
            }

            std::shared_ptr<IStack> preshared_public_key_mode(
                const Addresses& addresses,
                const InitiatorConfig& config,
                const openpal::Logger& logger,
                const std::shared_ptr<openpal::IExecutor>& executor,
                const CryptoSuite& crypto_suite,
                const StaticKeys& local_keys,
                const std::shared_ptr<const PublicKey>& remote_public_key)
            {
                return std::make_shared<InitiatorStack>(
                           addresses,
                           config,
                           logger,
                           executor,
                           InitiatorHandshakes::public_key_mode(
                               logger,
                               crypto_suite,
                               local_keys,
                               ICertificateHandler::preshared_key(remote_public_key)
                           )
                       );
            }

            std::shared_ptr<IStack> certificate_public_key_mode(
                const Addresses& addresses,
                const InitiatorConfig& config,
                const openpal::Logger& logger,
                const std::shared_ptr<openpal::IExecutor>& executor,
                const CryptoSuite& crypto_suite,
                const StaticKeys& local_keys,
                const std::shared_ptr<ssp21::SecureDynamicBuffer>& anchor_cert_file_data,
                const std::shared_ptr<ssp21::SecureDynamicBuffer>& presented_chain_file_data
            )
            {
                return std::make_shared<InitiatorStack>(
                           addresses,
                           config,
                           logger,
                           executor,
                           InitiatorHandshakes::public_key_mode(
                               logger,
                               crypto_suite,
                               local_keys,
                               ICertificateHandler::certificates(anchor_cert_file_data, presented_chain_file_data)
                           )
                       );
            }

        }
    }
}

