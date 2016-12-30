

#ifndef RESPONDER_FIXTURE_H
#define RESPONDER_FIXTURE_H

#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/Responder.h"

#include "testlib/MockExecutor.h"

#include "../mocks/MockLogHandler.h"
#include "../mocks/MockLowerLayer.h"
#include "../mocks/MockUpperLayer.h"
#include "../mocks/MockCryptoBackend.h"
#include "../mocks/HexMessageBuilders.h"

#include "ssp21/link/LinkConstants.h"

#include <memory>

namespace ssp21
{

    class ResponderFixture
    {
    private:

        struct MockFrameWriter : public IFrameWriter
        {
        public:

            MockFrameWriter(uint16_t max_payload_size) : max_payload_size(max_payload_size), buffer(max_payload_size) {}

            virtual WriteResult write(const IWritable& payload)  override
            {
                auto dest = buffer.as_wslice();
                const auto res = payload.write(dest);
                if (res.is_error()) return WriteResult::error(res.err);
                else
                {
                    return WriteResult::success(res, res.written);
                }
            }

            virtual uint16_t get_max_payload_size() const override
            {
                return max_payload_size;
            }

        private:

            uint16_t max_payload_size;
            openpal::Buffer buffer;
        };

        struct Keys
        {
            Keys(BufferType key_type) :
                local_kp(std::make_unique<KeyPair>()),
                remote_static_key(std::make_unique<PublicKey>())
            {
                init_key(local_kp->private_key, key_type);
                init_key(local_kp->public_key, key_type);
                init_key(*remote_static_key, key_type);
            }

            static void init_key(BufferBase& buffer, BufferType key_type)
            {
                buffer.as_wseq().set_all_to(0xFF);
                buffer.set_type(key_type);
            }

            std::unique_ptr<KeyPair> local_kp;
            std::unique_ptr<PublicKey> remote_static_key;
        };


    public:

        ResponderFixture(BufferType key_type = BufferType::x25519_key, std::unique_ptr<IFrameWriter> frame_writer = default_frame_writer(), const Responder::Config& config = Responder::Config()) :
            keys(key_type),
            log("responder"),
            exe(openpal::MockExecutor::Create()),
            lower(),
            responder(config, std::move(frame_writer), std::move(keys.local_kp), std::move(keys.remote_static_key), log.logger, exe, lower),
            upper(responder)
        {
            MockCryptoBackend::instance.clear_actions();
            responder.set_upper_layer(upper);
            this->lower.set_upper_layer(responder);
        }

        void set_tx_ready()
        {
            lower.set_tx_ready();
            responder.on_tx_ready();
        }

        static std::unique_ptr<IFrameWriter> default_frame_writer()
        {
            return std::make_unique<MockFrameWriter>(consts::link::max_config_payload_size);
        }

    private:

        Keys keys;

    public:

        ssp21::MockLogHandler log;
        std::shared_ptr<openpal::MockExecutor> exe;
        MockLowerLayer lower;
        Responder responder;
        MockUpperLayer upper;
    };

}

#endif

