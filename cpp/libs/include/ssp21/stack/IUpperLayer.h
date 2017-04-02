
#ifndef SSP21_IUPPERLAYER_H
#define SSP21_IUPPERLAYER_H

namespace ssp21
{
    /**
    * Performs asynchronous rx/tx operations on behalf of an ILowerLayer
    */
    class IUpperLayer
    {

    public:

        /**
        * open the layer if closed
        *
        * @return false if the layer is already open, true otherwise
        */
        inline bool on_lower_open()
        {
            if (this->is_open_flag)
            {
                return false;
            }
            else
            {
                this->is_open_flag = true;
                this->on_lower_open_impl();
                return true;
            }
        }

        /**
        * close the layer if open
        *
        * @return false if the layer is already closed, true otherwise
        */
        inline bool on_lower_close()
        {
            if (this->is_open_flag)
            {
                this->is_open_flag = false;
				this->on_lower_close_impl();
                return true;
            }
            else
            {
                return false;
            }
        }       

        /**
        * Called by the lower layer when ready to transmit data
        *
        * @return true if the layer is open, false if closed
        */
        inline bool on_lower_tx_ready()
        {
            if (this->is_open_flag)
            {
                this->on_lower_tx_ready_impl();
                return true;
            }
            else
            {
                return false;
            }
        }

		/**
		* Called by the lower layer when there is new data ready to receive
		*
		* @return true if the layer is open, false if closed
		*/
		inline bool on_lower_rx_ready()
		{
			if (this->is_open_flag)
			{
				this->on_lower_rx_ready_impl();
				return true;
			}
			else
			{
				return false;
			}
		}

        /**
        * @return true if the layer is open, false if closed.
        */
        inline bool is_open() const
        {
            return is_open_flag;
        }

    protected:

        virtual void on_lower_open_impl() = 0;

        virtual void on_lower_close_impl() = 0;

        virtual void on_lower_tx_ready_impl() = 0;

		virtual void on_lower_rx_ready_impl() = 0;

    private:

        bool is_open_flag = false;

    };
}

#endif