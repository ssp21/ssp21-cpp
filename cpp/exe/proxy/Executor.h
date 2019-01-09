#ifndef SSP21PROXY_EXECUTOR_H
#define SSP21PROXY_EXECUTOR_H

#include <exe4cpp/IExecutor.h>

#include <asio.hpp>
#include <chrono>

/**
* A simple, single-threaded executor based upon asio::io_context
*/
class Executor final : public exe4cpp::IExecutor, public std::enable_shared_from_this<Executor>
{

public:

    // @return a new timer based on a relative time duration
    virtual exe4cpp::ITimer* start(const exe4cpp::duration_t& duration, const exe4cpp::action_t& action) override;

    // @return a new timer based on an absolute timestamp of the monotonic clock
    virtual exe4cpp::ITimer* start(const exe4cpp::steady_time_t& expiration, const exe4cpp::action_t& action) override;

    // @return Thread-safe way to post an event to be handled asynchronously
    virtual void post(const exe4cpp::action_t& action) override;

    virtual exe4cpp::steady_time_t get_time() override;

    void run()
    {
        context.run();
    }

    asio::io_service& get_context()
    {
        return context;
    }

private:

    openpal::ITimer* start(const std::chrono::steady_clock::time_point& expiration, const openpal::action_t& runnable);

    asio::io_service context;

};

#endif

