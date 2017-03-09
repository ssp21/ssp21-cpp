
#ifndef SSP21_ISTACK_H
#define SSP21_ISTACK_H

#include "ILowerLayer.h"

namespace ssp21
{
    /**
     * A stack provides both lower/upper layers and can be bound to provided lower/upper layers
     */
    class IStack
    {
    public:

        virtual ~IStack() {}

        virtual ILowerLayer& get_lower() = 0;

        virtual IUpperLayer& get_upper() = 0;

        virtual void bind(ILowerLayer& lower, IUpperLayer& upper) = 0;
    };

}

#endif