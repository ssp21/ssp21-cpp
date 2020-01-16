#ifndef SSP21_MOCKLOWERLAYER_H
#define SSP21_MOCKLOWERLAYER_H

#include "ser4cpp/util/HexConversions.h"
#include "ser4cpp/util/Uncopyable.h"

#include "ssp21/stack/ILowerLayer.h"
#include "ssp21/stack/IUpperLayer.h"

#include <deque>

namespace ssp21 {
class MockLowerLayer : public ILowerLayer, private ser4cpp::Uncopyable {
    using message_t = ser4cpp::Buffer;

public:
    void bind_upper(IUpperLayer& upper)
    {
        this->upper = &upper;
    }

    virtual bool is_tx_ready() const override
    {
        return this->is_tx_ready_flag;
    }

    virtual bool start_tx_from_upper(const seq32_t& message) override
    {
        if (!this->is_tx_ready()) {
            throw std::logic_error("start_tx called when not tx ready");
        }

        this->tx_messages.push_back(std::make_unique<message_t>(message));

        return true;
    }

    virtual seq32_t start_rx_from_upper_impl() override
    {
        return this->rx_messages.empty() ? seq32_t::empty() : this->rx_messages.front()->as_rslice();
    }

    void enqueue_message(const std::string& hex)
    {
        this->rx_messages.push_back(ser4cpp::HexConversions::from_hex(hex));
        if (!this->is_upper_processing_rx()) {
            this->upper->on_lower_rx_ready();
        }
    }

    size_t num_rx_messages() const
    {
        return this->rx_messages.size();
    }

    std::string pop_tx_message()
    {
        if (this->tx_messages.empty()) {
            throw std::logic_error("No messages to pop()");
        }

        auto hex = ser4cpp::HexConversions::to_hex(this->tx_messages.front()->as_rslice());
        this->tx_messages.pop_front();
        return hex;
    }

    size_t num_tx_messages() const
    {
        return tx_messages.size();
    }

    void set_tx_ready(bool value)
    {
        this->is_tx_ready_flag = value;
    }

private:
    virtual void discard_rx_data() override
    {
        if (rx_messages.empty())
            throw std::logic_error("expected a frame to discard but rx_messages is empty");
        else {
            this->rx_messages.pop_front();
        }
    }

    IUpperLayer* upper = nullptr;

    bool is_tx_ready_flag = true;

    using message_queue_t = std::deque<std::unique_ptr<message_t>>;

    message_queue_t tx_messages;

    message_queue_t rx_messages;
};

}

#endif
