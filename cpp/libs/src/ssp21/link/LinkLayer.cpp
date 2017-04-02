
#include "ssp21/link/LinkLayer.h"

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
		this->upper->on_lower_rx_ready();
	}

	void LinkLayer::on_lower_rx_ready_impl()
	{
		if (this->remainder.is_empty() && this->try_read_from_lower())
		{
			this->upper->on_lower_rx_ready();
		}		
	}

	// ---- ILowerLayer -----

    bool LinkLayer::start_rx_from_upper_impl(seq32_t& data)
    {
		if (this->parser.has_result() || this->process_remainder() || this->try_read_from_lower())
		{
			LinkParser::Result result;
			parser.read(result);
			data = result.payload;
			return true;
		}
		
		return false;
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

	// ---- privat ehelpers -----

	bool LinkLayer::try_read_from_lower()
	{
		return this->lower->start_rx_from_upper(this->remainder) && this->process_remainder();
	}

	bool LinkLayer::process_remainder()
	{
		return this->remainder.is_empty() ? false : this->parser.parse(this->remainder);
	}
}
