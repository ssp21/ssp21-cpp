#ifndef SSP21_LINKLAYER_H
#define SSP21_LINKLAYER_H

#include "ssp21/stack/ILowerLayer.h"
#include "ssp21/stack/IUpperLayer.h"

#include "link/LinkParser.h"
#include "ssp21/link/LinkConstants.h"

namespace ssp21
{

    class LinkLayer final : public IUpperLayer, public ILowerLayer, private LinkParser::IReporter
    {

    public:

        LinkLayer(uint16_t local_addr, uint16_t remote_addr);

        void bind(ILowerLayer& lower, IUpperLayer& upper)
        {
            this->lower = &lower;
            this->upper = &upper;
        }

    private:

        // ---- IUpperLayer ----

        virtual void on_lower_open_impl() override;
        virtual void on_lower_close_impl() override;
        virtual void on_lower_tx_ready_impl() override;
        virtual void on_lower_rx_ready_impl() override;

        // ---- ILowerLayer ----
        virtual bool is_tx_ready() const override;
        virtual bool start_tx_from_upper(const seq32_t& data) override;
        virtual void discard_rx_data() override;
        virtual seq32_t start_rx_from_upper_impl() override;

        // ---- LinkParser::IReporter ----

        virtual void on_bad_header_crc(uint32_t expected, uint32_t actual) override {}
        virtual void on_bad_body_crc(uint32_t expected, uint32_t actual) override {}
        virtual void on_bad_body_length(uint32_t max_allowed, uint32_t actual) override {}

        // ---- private helpers ----

        bool get_frame();

        // return true to continue
        bool read_frame_one_iteration();

        bool parse(seq32_t& data);

        ILowerLayer* lower = nullptr;
        IUpperLayer* upper = nullptr;

        const uint16_t local_addr;
        const uint16_t remote_addr;

        seq32_t remainder;
        LinkParser parser;
    };

}


#endif
