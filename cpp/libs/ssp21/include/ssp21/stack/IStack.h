#ifndef SSP21_ISTACK_H
#define SSP21_ISTACK_H

/** @file
 * @brief Interface ssp21::IStack.
 */

#include "ILowerLayer.h"
#include "IUpperLayer.h"

namespace ssp21
{

/**
 * A stack provides both lower/upper layers and can be bound to provided lower/upper layers
 */
class IStack
{
public:
    virtual ~IStack() = default;

    /**
     * @brief Retrieve the @ref ILowerLayer of the stack.
     * @return Lower layer of the stack
     * 
     * The user @ref IUpperLayer should interact with this layer.
     */
    virtual ILowerLayer& get_lower() = 0;

    /**
     * @brief Retrieve the @ref IUpperLayer of the stack.
     * @return Upper layer of the stack
     * 
     * The user @ref ILowerLayer should interact with this layer.
     */
    virtual IUpperLayer& get_upper() = 0;

    /**
     * @brief Bind the user layers to the protocol stack.
     * @param lower Lower layer to bind
     * @param upper Upper layer to bind
     */
    virtual void bind(ILowerLayer& lower, IUpperLayer& upper) = 0;
};

}

#endif
