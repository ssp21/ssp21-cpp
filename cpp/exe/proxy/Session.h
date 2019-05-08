#ifndef SSP21PROXY_SESSION_H
#define SSP21PROXY_SESSION_H

#include "AsioLowerLayer.h"
#include "AsioUpperLayer.h"

#include <ssp21/stack/IStack.h>
#include <exe4cpp/IExecutor.h>

#include <functional>
#include <memory>

using session_error_handler_t = std::function<void()>;

class Session : public std::enable_shared_from_this<Session>
{

public:

    Session(
        uint64_t id,
        const session_error_handler_t& error_handler,
        const std::shared_ptr<exe4cpp::IExecutor>& executor,
        std::unique_ptr<IAsioSocketWrapper> lower_socket,
        std::unique_ptr<AsioLowerLayer> lower_layer,
        std::unique_ptr<IAsioSocketWrapper> upper_socket,
        std::unique_ptr<AsioUpperLayer> upper_layer,
        const std::shared_ptr<ssp21::IStack>& stack
    ) :
        id(id),
        error_handler(error_handler),
        executor(executor),
        lower_socket(std::move(lower_socket)),
        lower_layer(std::move(lower_layer)),
        upper_socket(std::move(upper_socket)),
        upper_layer(std::move(upper_layer)),
        stack(stack)
    {

    }

    static std::shared_ptr<Session> create(
        uint64_t id,
        const session_error_handler_t& error_handler,
        const std::shared_ptr<exe4cpp::IExecutor>& executor,
        std::unique_ptr<IAsioSocketWrapper> lower_socket,
        std::unique_ptr<AsioLowerLayer> lower_layer,
        std::unique_ptr<IAsioSocketWrapper> upper_socket,
        std::unique_ptr<AsioUpperLayer> upper_layer,
        const std::shared_ptr<ssp21::IStack>& stack
    )
    {
        return std::make_shared<Session>(id,
                                         error_handler,
                                         executor,
                                         std::move(lower_socket),
                                         std::move(lower_layer),
                                         std::move(upper_socket),
                                         std::move(upper_layer),
                                         stack);
    }

    void start()
    {
        stack->bind(*(lower_layer), *(upper_layer));
        upper_layer->bind(*upper_socket, stack->get_lower(), error_handler);
        lower_layer->open(*lower_socket, stack->get_upper());
    }

    void shutdown()
    {
        lower_layer->close(); // start the shutdown bottom to top
        this->check_for_close(this->shared_from_this());
    }

private:

    void check_for_close(const std::shared_ptr<Session>& self)
    {
        if (!self->is_shutdown())
        {
            self->executor->post([self]()
            {
                self->check_for_close(self);
            });
        }
    }

    inline bool is_shutdown() const
    {
        return !(this->lower_layer->is_active() || this->upper_layer->is_active());
    }

    const uint64_t id;
    const session_error_handler_t error_handler;
    const std::shared_ptr<exe4cpp::IExecutor> executor;
    const std::unique_ptr<IAsioSocketWrapper> lower_socket;
    const std::unique_ptr<AsioLowerLayer> lower_layer;
    const std::unique_ptr<IAsioSocketWrapper> upper_socket;
    const std::unique_ptr<AsioUpperLayer> upper_layer;
    const std::shared_ptr<ssp21::IStack> stack;
};

#endif
