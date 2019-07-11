#ifndef SSP21PROXY_IASIOSOCKETWRAPPER_H
#define SSP21PROXY_IASIOSOCKETWRAPPER_H

#include <ssp21/util/SequenceTypes.h>

class IAsioSocketWrapper
{

public:

    virtual ~IAsioSocketWrapper() = default;

    virtual bool start_rx_from_socket() = 0;
    virtual bool start_tx_to_socket(const ssp21::seq32_t& data) = 0;
    virtual bool try_close_socket() = 0;

    virtual bool get_is_tx_active() const = 0;
    virtual bool get_is_rx_active() const = 0;

    inline bool is_active() const
    {
        return this->get_is_rx_active() || this->get_is_tx_active();
    }
};

#endif
