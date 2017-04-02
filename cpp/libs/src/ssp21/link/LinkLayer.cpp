
#include "ssp21/link/LinkLayer.h"

namespace ssp21
{

    LinkLayer::LinkLayer(uint16_t local_addr, uint16_t remote_addr) :
        local_addr(local_addr),
        remote_addr(remote_addr),
        parser(ssp21::consts::link::max_config_payload_size, *this)
    {}

    void LinkLayer::on_open_from_lower_impl()
    {
        upper->on_open_from_lower();
    }

    void LinkLayer::on_close_impl()
    {
        upper->on_close();
    }

    void LinkLayer::on_tx_ready_impl()
    {
        upper->on_tx_ready();
    }

    void LinkLayer::start_rx_impl(const seq32_t& data)
    {
        this->remainder = data;
        this->process_remainder();
    }

    void LinkLayer::process_remainder()
    {
        if (this->parser.parse(this->remainder) && this->parser.read(this->result))
        {
            this->try_start_upper_rx(this->result.payload);
        }

        if (this->is_rx_ready_impl())
        {
            lower->on_rx_ready();
        }
    }

    bool LinkLayer::is_rx_ready_impl()
    {
        return this->result.payload.is_empty() && !this->is_rx_processing() && this->remainder.is_empty();
    }

    bool LinkLayer::is_tx_ready() const
    {
        return this->lower->is_tx_ready();
    }

    void LinkLayer::discard_rx_data()
    {
        this->result.payload.make_empty();
    }

    bool LinkLayer::start_tx(const seq32_t& data)
    {
        // The upper layer formats the frame for efficiency purposes
        return this->lower->start_tx(data);
    }

    void LinkLayer::on_rx_ready_impl()
    {
        if (this->remainder.is_not_empty())
        {
            this->process_remainder();
        }
        else
        {
            this->lower->on_rx_ready();
        }
    }

}
