
#ifndef SSP21_SHAREDSECRETRESPONDERHANDSHAKE_H
#define SSP21_SHAREDSECRETRESPONDERHANDSHAKE_H

#include "ssp21/crypto/IResponderHandshake.h"
#include "ssp21/crypto/BufferTypes.h"

namespace ssp21
{

    class SharedSecretResponderHandshake : public IResponderHandshake
    {
    public:

        SharedSecretResponderHandshake(const openpal::Logger& logger, const std::shared_ptr<SymmetricKey>& shared_secret);

        inline static std::shared_ptr<IResponderHandshake> create_shared(openpal::Logger logger, const std::shared_ptr<SymmetricKey>& shared_secret)
        {
            return std::make_shared<SharedSecretResponderHandshake>(logger, shared_secret);
        }

        virtual IResponderHandshake::Result process(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now, IFrameWriter& writer, Session& session) override;

    private:

        openpal::Logger logger;
        const std::shared_ptr<SymmetricKey> shared_secret;
    };



}

#endif
