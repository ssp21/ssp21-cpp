
#ifndef SSP21_LINKCRYPTOSTACK_H
#define SSP21_LINKCRYPTOSTACK_H

#include "link/LinkLayer.h"
#include "crypto/Responder.h"
#include "crypto/Initiator.h"
#include "link/LinkFrameWriter.h"
#include "ssp21/stack/IStack.h"
#include "RawFrameWriter.h"

namespace ssp21
{
    class AbstractStack : public IStack
    {
    public:
        AbstractStack(ILowerLayer& lower, IUpperLayer& upper)
            : lower(lower),
              upper(upper)
        {}

        virtual ~AbstractStack() = default;

        // ILowerLayer
        virtual bool is_tx_ready() const
        {
            return lower.is_tx_ready();
        }

        virtual bool start_tx_from_upper(const seq32_t& data)
        {
            return lower.start_tx_from_upper(data);
        }

        virtual void discard_rx_data()
        {
            lower.discard_rx_data();
        }

        virtual seq32_t start_rx_from_upper_impl()
        {
            return lower.start_rx_from_upper_impl();
        }

        // IUpperLayer
        virtual void on_lower_open_impl()
        {
            upper.on_lower_open();
        }

        virtual void on_lower_close_impl()
        {
            upper.on_lower_close();
        }

        virtual void on_lower_tx_ready_impl()
        {
            upper.on_lower_tx_ready();
        }

        virtual void on_lower_rx_ready_impl()
        {
            upper.on_lower_rx_ready();
        }

    private:
        ILowerLayer& lower;
        IUpperLayer& upper;
    };

    class FullResponderStack final : public AbstractStack
    {
    public:

        FullResponderStack(
            Addresses addresses,
            const ResponderConfig& config,
            log4cpp::Logger logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const std::shared_ptr<IResponderHandshake>& handshake
        ) :
            link(addresses.source, addresses.destination),
            responder(
                config,
                logger,
                get_frame_writer(logger, addresses, consts::link::max_config_payload_size),
                executor,
                handshake
            ),
            AbstractStack(responder, link)
        {

        }

        void bind(ILowerLayer& lower, IUpperLayer& upper) override
        {
            this->link.bind(lower, responder);
            this->responder.bind(link, upper);
        }

    private:

        static std::shared_ptr<IFrameWriter> get_frame_writer(log4cpp::Logger logger, Addresses addresses, uint16_t max_payload_size)
        {
            return std::make_shared<LinkFrameWriter>(logger, addresses, max_payload_size);
        }

        LinkLayer link;
        Responder responder;
    };

    class CryptoOnlyResponderStack final : public AbstractStack
    {
    public:

        CryptoOnlyResponderStack(
            const ResponderConfig& config,
            log4cpp::Logger logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const std::shared_ptr<IResponderHandshake>& handshake
        ) :
            responder(
                config,
                logger,
                get_frame_writer(logger, consts::link::max_config_payload_size),
                executor,
                handshake
            ),
            AbstractStack(responder, responder)
        {

        }

        void bind(ILowerLayer& lower, IUpperLayer& upper) override
        {
            this->responder.bind(lower, upper);
        }

    private:

        static std::shared_ptr<IFrameWriter> get_frame_writer(log4cpp::Logger logger, uint16_t max_payload_size)
        {
            return std::make_shared<RawFrameWriter>(logger, max_payload_size);
        }

        Responder responder;
    };

    class FullInitiatorStack final : public AbstractStack
    {
    public:

        FullInitiatorStack(
            Addresses addresses,
            const InitiatorConfig& config,
            log4cpp::Logger logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const std::shared_ptr<IInitiatorHandshake>& handshake) :
            link(addresses.source, addresses.destination),
            initiator(
                config,
                logger,
                get_frame_writer(logger, addresses, consts::link::max_config_payload_size),
                executor,
                handshake
            ),
            AbstractStack(initiator, link)
        {

        }

        void bind(ILowerLayer& lower, IUpperLayer& upper) override
        {
            this->link.bind(lower, initiator);
            this->initiator.bind(link, upper);
        }

    private:

        static std::shared_ptr<IFrameWriter> get_frame_writer(log4cpp::Logger logger, Addresses addresses, uint16_t max_payload_size)
        {
            return std::make_shared<LinkFrameWriter>(logger, addresses, max_payload_size);
        }

        LinkLayer link;
        Initiator initiator;
    };

    class CryptoOnlyInitiatorStack final : public AbstractStack
    {
    public:

        CryptoOnlyInitiatorStack(
            const InitiatorConfig& config,
            log4cpp::Logger logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const std::shared_ptr<IInitiatorHandshake>& handshake) :
            initiator(
                config,
                logger,
                get_frame_writer(logger, consts::link::max_config_payload_size),
                executor,
                handshake
            ),
            AbstractStack(initiator, initiator)
        {

        }

        void bind(ILowerLayer& lower, IUpperLayer& upper) override
        {
            this->initiator.bind(lower, upper);
        }

    private:

        static std::shared_ptr<IFrameWriter> get_frame_writer(log4cpp::Logger logger, uint16_t max_payload_size)
        {
            return std::make_shared<RawFrameWriter>(logger, max_payload_size);
        }

        Initiator initiator;
    };
}

#endif
