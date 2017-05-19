#include "Executor.h"



using namespace openpal;
using namespace std::chrono;

class Timer : public ITimer
{
    friend class Executor;

public:

    Timer(asio::io_service& service) : timer(service)
    {}

    virtual void cancel() override
    {
        timer.cancel();
    }

    virtual Timestamp expires_at() override
    {
        return Timestamp(
                   duration_cast<milliseconds>(timer.expires_at().time_since_epoch()).count()
               );
    }

private:

    asio::basic_waitable_timer<steady_clock> timer;
};


ITimer* Executor::start(const TimeDuration& delay, const action_t& action)
{
    const auto now = steady_clock::now();
    const auto max_ms = duration_cast<milliseconds>(steady_clock::time_point::max() - now).count();
    const steady_clock::time_point expiration = (delay.get_milliseconds() > max_ms) ? steady_clock::time_point::max() : now + milliseconds(delay.get_milliseconds());

    return start(expiration, action);
}

ITimer* Executor::start(const Timestamp& expiration, const action_t& action)
{
    const auto max_ms = duration_cast<std::chrono::milliseconds>(steady_clock::duration::max()).count();
    const auto time_point = (expiration.milliseconds > max_ms) ? steady_clock::time_point::max() : steady_clock::time_point(milliseconds(expiration.milliseconds));

    return this->start(time_point, action);
}

ITimer* Executor::start(const steady_clock::time_point& expiration, const openpal::action_t& action)
{
    const auto timer = std::make_shared<Timer>(this->context);

    timer->timer.expires_at(expiration);

    // neither the executor nor the timer can be deleted while the timer is still active
    auto callback = [timer, action, self = shared_from_this()](const std::error_code & ec)
    {
        if (!ec)   // an error indicate timer was canceled
        {
            action();
        }
    };

    timer->timer.async_wait(callback);

    return timer.get();
}

void Executor::post(const action_t& action)
{
    this->context.post(action);
}

Timestamp Executor::get_time()
{
    return Timestamp(
               duration_cast<std::chrono::milliseconds>(steady_clock::now().time_since_epoch()).count()
           );
}


