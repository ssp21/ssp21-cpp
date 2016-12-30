
#ifndef SSP21_TXSTATE_H
#define SSP21_TXSTATE_H

#include "ssp21/crypto/Session.h"

#include "openpal/util/Uncopyable.h"

namespace ssp21
{
    /**
    	Tracks the state of a multi-segment transmission for the crypto layer
    */
    class TxState final : openpal::Uncopyable
    {

    public:

        TxState() {}

        void reset()
        {
            this->fir = false;
            this->transmitting = false;
            this->remainder.make_empty();
        }

        bool initialize(const seq32_t& data)
        {
            if (transmitting) return false;

            this->fir = true;
            this->transmitting = false;
            this->remainder = data;

            return true;
        }

        void on_tx_complete()
        {
            this->transmitting = false;
        }

        bool begin_transmit(const seq32_t& remainder)
        {
            if (!transmitting) return false;

            this->fir = false;
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

        bool get_fir() const
        {
            return fir;
        }

    private:

        bool fir = false;
        bool transmitting = false;
        seq32_t remainder;

    };

}

#endif
