
#ifndef SSP21_QKDRESPONDERHANDSHAKE_H
#define SSP21_QKDRESPONDERHANDSHAKE_H

#include "crypto/IResponderHandshake.h"
#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/IKeyLookup.h"

namespace ssp21
{

    class QKDResponderHandshake final : public IResponderHandshake
    {
    public:

        QKDResponderHandshake(const openpal::Logger& logger, const std::shared_ptr<IKeyLookup>& key_lookup);

        inline static std::shared_ptr<IResponderHandshake> make_shared(openpal::Logger logger, const std::shared_ptr<IKeyLookup>& key_lookup)
        {
            return std::make_shared<QKDResponderHandshake>(logger, key_lookup);
        }

        virtual IResponderHandshake::Result process(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now, IFrameWriter& writer, Session& session) override;

    private:

        openpal::Logger logger;
        const std::shared_ptr<IKeyLookup> key_lookup;
    };



}

#endif
