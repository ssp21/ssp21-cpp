#ifndef SSP21_ILOWERLAYER_H
#define SSP21_ILOWERLAYER_H

/** @file
 * @brief Interface @ref ssp21::ILowerLayer.
 */

#include "ssp21/util/SequenceTypes.h"

namespace ssp21 {

/**
 * @brief Performs asynchronous RX/TX operations on behalf of an @ref IUpperLayer.
 * 
 * This class is used by @ref IStack. User of the library must implement this interface
 * to send and receive the bytes on the wire.
 */
class ILowerLayer {
    friend class AbstractStack;

public:
    /**
     * @brief Check if the layer is ready to transmit data.
     * @return @cpp true @ce if a call to @ref start_tx_from_upper() will succeed, @cpp false @ce otherwise.
     */
    virtual bool is_tx_ready() const = 0;

    /**
     * @brief Start an asynchronous TX operation.
     * @param data Bytes to be transmitted
     * @return @cpp true @ce if the operation was successfully executed or queued, @cpp false @ce otherwise.
     * 
     * Once the operation is successfully completed, the implementor must call @ref IUpperLayer::on_lower_tx_ready().
     * 
     * The underlying buffer pointed to by `data` is loaned out to this layer
     * and must not be mutated until @ref IUpperLayer::on_lower_tx_ready() is called.
     */
    virtual bool start_tx_from_upper(const seq32_t& data) = 0;

    /**
     * @brief Called by the @ref IUpperLayer when it's ready to receive the next chunk of data.
     * @return Slice of received data
     * 
     * The returned slice must remain valid until the next call or until the @ref IUpperLayer
     * is closed.
     * 
     * Implementor of this class must implement @ref ILowerLayer::start_rx_from_upper_impl().
     */
    inline seq32_t start_rx_from_upper()
    {
        if (this->upper_is_processing_rx_data) {
            this->discard_rx_data();
            this->upper_is_processing_rx_data = false;
        }

        const auto ret = this->start_rx_from_upper_impl();

        if (ret.is_not_empty()) {
            this->upper_is_processing_rx_data = true;
        }

        return ret;
    }

protected:
    /**
     * @brief Called when a previous @ref ILowerLayer::start_rx_from_upper() operation completes.
     * 
     * Notifies that the buffer containing the RX data can be flushed.
     */
    virtual void discard_rx_data() = 0;

    /**
     * @brief Start reading data.
     * @return Slice of data
     * 
     * When bytes arrive, the implementor must call @ref IUpperLayer::on_lower_rx_ready().
     * 
     * The returned slice must remain valid until the next call or until the @ref IUpperLayer
     * is closed.
     * 
     *  See @ref ILowerLayer::start_rx_from_upper()
     */
    virtual seq32_t start_rx_from_upper_impl() = 0;

    /**
     * @brief Reset the layer
     * 
     * This method cancels all pending RX and TX operations.
     */
    inline void reset_this_lower_layer()
    {
        this->upper_is_processing_rx_data = false;
    }

    /**
     * @brief Check if the upper layer is waiting for RX data.
     * @return @cpp true @ce if the upper layer is waiting for RX data, @cpp false @ce otherwise.
     */
    inline bool is_upper_processing_rx() const
    {
        return upper_is_processing_rx_data;
    }

private:
    bool upper_is_processing_rx_data = false;
};

}

#endif
