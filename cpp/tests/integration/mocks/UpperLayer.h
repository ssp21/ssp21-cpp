#ifndef SSP21_UPPERLAYER_H
#define SSP21_UPPERLAYER_H

#include "ssp21/stack/IUpperLayer.h"
#include "ssp21/stack/ILowerLayer.h"

#include <memory>
#include <vector>

namespace ssp21
{
    struct IReceiveValidator
    {
        virtual void validate(const seq32_t& data) = 0;
    };

    class UpperLayer final : public IUpperLayer
    {

    public:

        void configure(ILowerLayer& lower)
        {
            this->lower = &lower;
        }

        uint32_t num_bytes_rx = 0;

        void add_validator(const std::shared_ptr<IReceiveValidator>& validator)
        {
            this->validators.push_back(validator);
        }

    private:

        virtual void on_lower_open_impl() override {}

        virtual void on_lower_close_impl() override {}

        virtual void on_lower_tx_ready_impl() override {}

        virtual void on_lower_rx_ready_impl() override
        {
            while (read_one());
        }

        bool read_one()
        {
            const auto data = lower->start_rx_from_upper();
            if (data)
            {
                this->num_bytes_rx += data.length();

                for (auto& v : this->validators)
                {
                    v->validate(data);
                }

                return true;
            }

            return false;
        }

        // set during configure step
        ILowerLayer* lower = nullptr;

        std::vector<std::shared_ptr<IReceiveValidator>> validators;
    };

}

#endif
