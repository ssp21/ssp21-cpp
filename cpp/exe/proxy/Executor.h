#ifndef SSP21PROXY_EXECUTOR_H
#define SSP21PROXY_EXECUTOR_H

#include <openpal/executor/IExecutor.h>

#include <asio.hpp>
#include <chrono>

/**
* A simple, single-threaded executor based upon asio::io_context
*/
class Executor final : public openpal::IExecutor, public std::enable_shared_from_this<Executor>
{

public:

    // @return a new timer based on a relative time duration
    virtual openpal::ITimer* start(const openpal::TimeDuration& duration, const openpal::action_t& action) override;

    // @return a new timer based on an absolute timestamp of the monotonic clock
    virtual openpal::ITimer* start(const openpal::Timestamp& expiration, const openpal::action_t& action) override;

    // @return Thread-safe way to post an event to be handled asynchronously
    virtual void post(const openpal::action_t& action) override;

    virtual openpal::Timestamp get_time() override;

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

