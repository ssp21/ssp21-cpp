#ifndef SSP21_ISTACK_H
#define SSP21_ISTACK_H

/** @file
 * @brief Interface ssp21::IStack.
 */

#include "ILowerLayer.h"
#include "IUpperLayer.h"

namespace ssp21 {

/**
 * A stack provides both lower/upper layers and can be bound to provided lower/upper layers
 */
class IStack : public ILowerLayer, public IUpperLayer {
public:
    virtual ~IStack() = default;

    /**
     * @brief Bind the user layers to the protocol stack.
     * @param lower Lower layer to bind
     * @param upper Upper layer to bind
     */
    virtual void bind(ILowerLayer& lower, IUpperLayer& upper) = 0;
};

}

#endif
