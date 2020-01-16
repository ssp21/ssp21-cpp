#ifndef SSP21_MOCKUPPERLAYER_H
#define SSP21_MOCKUPPERLAYER_H

#include "ser4cpp/util/HexConversions.h"
#include "ser4cpp/util/Uncopyable.h"

#include "ssp21/stack/IUpperLayer.h"

#include <deque>

namespace ssp21 {
class MockUpperLayer : public IUpperLayer, private ser4cpp::Uncopyable {

public:
    void bind_lower(ILowerLayer& lower)
    {
        this->lower = &lower;
    }

    std::string pop_rx_message()
    {
        if (this->rx_messages.empty()) {
            throw std::logic_error("No rx messages");
        }

        auto hex = this->rx_messages.front();
        this->rx_messages.pop_front();
        return hex;
    }

    bool is_empty() const
    {
        return this->rx_messages.empty();
    }

    bool get_is_open() const
    {
        return this->is_open;
    }

    uint32_t num_tx_ready = 0;

private:
    bool is_open = false;

    using message_queue_t = std::deque<std::string>;

    message_queue_t rx_messages;

    ILowerLayer* lower = nullptr;

    virtual void on_lower_open_impl() override
    {
        this->is_open = true;
    }

    virtual void on_lower_close_impl() override
    {
        this->is_open = false;
    }

    virtual void on_lower_tx_ready_impl() override
    {
        ++num_tx_ready;
    }

    virtual void on_lower_rx_ready_impl() override
    {
        // read all available data
        const auto data = this->lower->start_rx_from_upper();
        if (data.is_not_empty()) {
            this->rx_messages.push_back(ser4cpp::HexConversions::to_hex(data));
            this->on_lower_rx_ready_impl();
        }
    }
};

}

#endif
