
#ifndef SSP21_TXSTATE_H
#define SSP21_TXSTATE_H

#include "ssp21/crypto/Session.h"

#include "openpal/util/Uncopyable.h"

namespace ssp21
{
    /**
    	Tracks the state of a transmission from the crypto layer
    */
    class TxState final : openpal::Uncopyable
    {

    public:

        TxState() {}

        void reset()
        {
            this->transmitting = false;
            this->remainder.make_empty();
        }

        void initialize(const seq32_t& data)
        {
            this->transmitting = false;
            this->remainder = data;
        }

        bool is_idle() const
        {
            return !this->transmitting && this->remainder.is_empty();
        }

        bool on_tx_complete()
        {
            if (this->transmitting)
            {
                this->transmitting = false;

                // if there's nothing left to transmit, the entire tx sequence is complete
                return this->remainder.is_empty();
            }

            return false;
        }

        bool begin_transmit(const seq32_t& remainder)
        {
            if (this->remainder.is_empty()) return false;

            this->transmitting = true;
            this->remainder = remainder;

            return true;
        }

        // ---- accessors -----

        bool is_active() const
        {
            return transmitting || remainder.is_not_empty();
        }

        bool is_ready_tx() const
        {
            return !transmitting && remainder.is_not_empty();
        }

        bool is_transmitting() const
        {
            return transmitting;
        }

        seq32_t get_remainder() const
        {
            return remainder;
        }

    private:

        bool transmitting = false;
        seq32_t remainder;

    };

}

#endif
