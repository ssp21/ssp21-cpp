
#ifndef SSP21_SHAREDSECRETRESPONDERHANDSHAKE_H
#define SSP21_SHAREDSECRETRESPONDERHANDSHAKE_H

#include "ssp21/crypto/IResponderHandshake.h"
#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/IKeyLookup.h"

namespace ssp21
{

    class SharedSecretResponderHandshake : public IResponderHandshake
    {
    public:

        SharedSecretResponderHandshake(const openpal::Logger& logger, const std::shared_ptr<IKeyLookup>& key_lookup);

        inline static std::shared_ptr<IResponderHandshake> make_shared(openpal::Logger logger, const std::shared_ptr<IKeyLookup>& key_lookup)
        {
            return std::make_shared<SharedSecretResponderHandshake>(logger, key_lookup);
        }

        virtual IResponderHandshake::Result process(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now, IFrameWriter& writer, Session& session) override;

    private:

        openpal::Logger logger;
        const std::shared_ptr<IKeyLookup> key_lookup;
    };



}

#endif
