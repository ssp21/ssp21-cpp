
#ifndef SSP21_ILOWERLAYER_H
#define SSP21_ILOWERLAYER_H

#include "ssp21/util/SequenceTypes.h"

namespace ssp21
{
    /**
    * Performs asynchronous rx/tx operations on behalf of an IUpperLayer
    */
    class ILowerLayer
    {

    public:

		/**
		*   Check the transmit readiness of the layer
		*
		*   @return true if ready to start_tx(..) will succeed, false if it will fail
		*/
		virtual bool is_tx_ready() const = 0;

        /**
        *
        *   Start an asynchronous tx operation. The underlying buffer pointed to by 'data'
        *   is loaned out to this layer and must not be mutated until IUpperLayer::on_tx_ready()
        *   is called.
        *
        *   @param data bytes to be transmitted
        */
        virtual bool start_tx_from_upper(const seq32_t& data) = 0;       

        /**
        *  Called by the upper layer when its ready to receive the next chunk of data. Any previously received slices are no longer valid, 
		*  and the underlying buffers may once again be mutated.z
		* 
		*  @param (output) reference to the slice of data if return value is true
		*  @return true if there is data to be read, false otherwise
		*
        */
        inline bool start_rx_from_upper(seq32_t& data)
        {
            if (this->upper_is_processing_rx_data)
            {
                this->discard_rx_data();
                this->upper_is_processing_rx_data = false;
            }

			if (this->start_rx_from_upper_impl(data))
			{
				this->upper_is_processing_rx_data = true;
				return true;
			}

			return false;
        }

    protected:        

        /**
        * Called when a previous start_rx_from_upper operation completes
        */
        virtual void discard_rx_data() = 0;

        virtual bool start_rx_from_upper_impl(seq32_t& data) = 0;

        inline void reset_this_lower_layer()
        {
            this->upper_is_processing_rx_data = false;
        }

        inline bool is_upper_processing_rx() const
        {
            return upper_is_processing_rx_data;
        }

    private:

        /**
        * Tracks if the upper layer is currently processing some data
        */
        bool upper_is_processing_rx_data = false;

    };
}

#endif
