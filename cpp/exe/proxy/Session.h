#ifndef SSP21PROXY_SESSION_H
#define SSP21PROXY_SESSION_H

#include "ASIOLowerLayer.h"
#include "ASIOUpperLayer.h"

#include <ssp21/stack/IStack.h>
#include <exe4cpp/IExecutor.h>

#include <functional>

typedef ASIOLowerLayer socket_lower_layer_t;
typedef ASIOUpperLayer socket_upper_layer_t;
typedef std::function<void()> session_error_handler_t;

class Session : public std::enable_shared_from_this<Session>
{

public:

    Session(
        uint64_t id,
        const session_error_handler_t& error_handler,
        const std::shared_ptr<exe4cpp::IExecutor>& executor,
        std::unique_ptr<socket_lower_layer_t> lower,
        std::unique_ptr<socket_upper_layer_t> upper,
        const std::shared_ptr<ssp21::IStack>& stack
    ) :
        id(id),
        error_handler(error_handler),
        executor(executor),
        lower(std::move(lower)),
        upper(std::move(upper)),
        stack(stack)
    {

    }

    static std::shared_ptr<Session> create(
        uint64_t id,
        const session_error_handler_t& error_handler,
        const std::shared_ptr<exe4cpp::IExecutor>& executor,
        std::unique_ptr<socket_lower_layer_t> lower,
        std::unique_ptr<socket_upper_layer_t> upper,
        const std::shared_ptr<ssp21::IStack>& stack
    )
    {
        return std::make_shared<Session>(id, error_handler, executor, std::move(lower), std::move(upper), stack);
    }

    void start()
    {
        stack->bind(*(lower), *(upper));
        upper->bind(stack->get_lower(), error_handler);
        lower->open(stack->get_upper());
    }

    void shutdown()
    {
        lower->close(); // start the shutdown bottom to top
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
        return !(this->lower->is_active() || this->upper->is_active());
    }

    const uint64_t id;
    const session_error_handler_t error_handler;
    const std::shared_ptr<exe4cpp::IExecutor> executor;
    const std::unique_ptr<socket_lower_layer_t> lower;
    const std::unique_ptr<socket_upper_layer_t> upper;
    const std::shared_ptr<ssp21::IStack> stack;
};

#endif
