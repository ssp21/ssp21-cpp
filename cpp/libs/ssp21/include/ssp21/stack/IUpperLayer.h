#ifndef SSP21_IUPPERLAYER_H
#define SSP21_IUPPERLAYER_H

/** @file
 * @brief Interface @ref ssp21::IUpperLayer.
 */

namespace ssp21
{

/**
 * @brief Performs asynchronous RX/TX operations on behalf of an @ref ILowerLayer.
 * 
 * This class is used by @ref IStack. User of the library must implement this interface
 * to send and receive the bytes to the host application.
 */
class IUpperLayer
{
public:
    /**
     * @brief Open the layer if it was closed.
     * @return @cpp true @ce if the layer was originally closed, @cpp false @ce otherwise.
     * 
     * Implementor of this class must implement @ref IUpperLayer::on_lower_open_impl().
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
     * @brief Close the layer if it was open.
     * @return @cpp true @ce if the layer was originally open, @cpp false @ce otherwise.
     * 
     * Implementor of this class must implement @ref IUpperLayer::on_lower_close_impl().
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
     * @brief Called by the @ref ILowerLayer when it's ready to transmit data.
     * @return @cpp true @ce if the layer is open, @cpp false @ce otherwise.
     * 
     * Implementor of this class must implement @ref IUpperLayer::on_lower_tx_ready_impl().
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
     * @brief Called by the @ref ILowerLayer when new data is available.
     * @return @cpp true @ce if the layer is open, @cpp false @ce otherwise.
     * 
     * Implementor of this class must implement @ref IUpperLayer::on_lower_rx_ready_impl().
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
     * @brief Check if layer is currently open.
     * @return @cpp true @ce if the layer is open, @cpp false @ce otherwise.
     */
    inline bool is_open() const
    {
        return is_open_flag;
    }

protected:
    /**
     * @brief Open the layer.
     * 
     * See @ref IUpperLayer::on_lower_open()
     */
    virtual void on_lower_open_impl() = 0;

    /**
     * @brief Close the layer.
     * 
     * See @ref IUpperLayer::on_lower_close()
     */
    virtual void on_lower_close_impl() = 0;

    /**
     * @brief Callback when @ref ILowerLayer is ready to transmit data.
     * 
     * See @ref IUpperLayer::on_lower_tx_ready()
     */
    virtual void on_lower_tx_ready_impl() = 0;

    /**
     * @brief Callback when @ref ILowerLayer received data.
     * 
     * See @ref IUpperLayer::on_lower_rx_ready()
     */
    virtual void on_lower_rx_ready_impl() = 0;

private:
    bool is_open_flag = false;
};

}

#endif
