
#include "link/LinkLayer.h"

namespace ssp21
{

    LinkLayer::LinkLayer(uint16_t local_addr, uint16_t remote_addr) :
        local_addr(local_addr),
        remote_addr(remote_addr),
        parser(ssp21::consts::link::max_config_payload_size, *this)
    {}

    // ---- IUpperLayer -----

    void LinkLayer::on_lower_open_impl()
    {
        this->upper->on_lower_open();
    }

    void LinkLayer::on_lower_close_impl()
    {
        this->parser.reset();
        this->upper->on_lower_close();
    }

    void LinkLayer::on_lower_tx_ready_impl()
    {
        this->upper->on_lower_tx_ready();
    }

    void LinkLayer::on_lower_rx_ready_impl()
    {
        if (this->get_frame()) this->upper->on_lower_rx_ready();
    }

    // ---- ILowerLayer -----

    seq32_t LinkLayer::start_rx_from_upper_impl()
    {
        if (this->get_frame())
        {
            LinkParser::Result result;
            parser.read(result);
            if(result.destination == local_addr)
            {
                return result.payload;
            }
        }

        return seq32_t::empty();
    }

    bool LinkLayer::is_tx_ready() const
    {
        return this->lower->is_tx_ready();
    }

    void LinkLayer::discard_rx_data()
    {
        this->parser.reset();
    }

    bool LinkLayer::start_tx_from_upper(const seq32_t& data)
    {
        // The upper layer formats the frame for efficiency purposes
        return this->lower->start_tx_from_upper(data);
    }

    // ---- private helpers -----

    bool LinkLayer::get_frame()
    {
        if (this->parser.has_result()) return true;

        while (this->read_frame_one_iteration());

        return this->parser.has_result();
    }

    // return true to continue
    bool LinkLayer::read_frame_one_iteration()
    {
        if (this->remainder.is_empty())
        {
            this->remainder = this->lower->start_rx_from_upper();
            return this->remainder.is_not_empty();
        }
        else
        {
            return !this->parse(this->remainder);
        }
    }

    bool LinkLayer::parse(seq32_t& data)
    {
        return data.is_empty() ? false : this->parser.parse(data);
    }
}
