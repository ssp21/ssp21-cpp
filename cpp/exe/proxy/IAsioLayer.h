#ifndef SSP21PROXY_IASIOLAYER_H
#define SSP21PROXY_IASIOLAYER_H

#include <ssp21/util/SequenceTypes.h>

class IAsioLayer
{

public:

    virtual ~IAsioLayer() = default;

    virtual void on_rx_complete(const ssp21::seq32_t& data) = 0;
    virtual void on_tx_complete() = 0;
    virtual void on_rx_or_tx_error() = 0;
    virtual bool is_active() const = 0;
};

#endif
