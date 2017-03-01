#ifndef SSP21_LINKLAYER_H
#define SSP21_LINKLAYER_H

#include "ssp21/LayerInterfaces.h"
#include "ssp21/link/LinkParser.h"

#include "ssp21/link/LinkConstants.h"

namespace ssp21
{

    class LinkLayer final : public IDualLayer, private LinkParser::IReporter
    {

    public:

        LinkLayer(uint16_t local_addr, uint16_t remote_addr);

        void bind(ILowerLayer& lower, IUpperLayer& upper) override
        {
            this->lower = &lower;
            this->upper = &upper;
        }

    private:

        // ---- link_upper_layer_t ----

        virtual void on_open_impl() override;
        virtual void on_close_impl() override;
        virtual void on_tx_ready_impl() override;
        virtual void start_rx_impl(const seq32_t& data) override;


        // ---- ILowerLayer ----

        virtual bool start_tx(const seq32_t& data) override;
        virtual void on_rx_ready_impl() override;

        // ---- LinkParser::IReporter ----

        virtual void on_bad_header_crc(uint32_t expected, uint32_t actual) override {}
        virtual void on_bad_body_crc(uint32_t expected, uint32_t actual) override {}
        virtual void on_bad_body_length(uint32_t max_allowed, uint32_t actual) override {}

        IUpperLayer* upper = nullptr;
        ILowerLayer* lower = nullptr;

        const uint16_t local_addr;
        const uint16_t remote_addr;

        LinkParser::Result result;

        LinkParser parser;
    };

}


#endif
