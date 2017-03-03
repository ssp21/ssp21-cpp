
#ifndef SSP21_LAYER_INTERFACES_H
#define SSP21_LAYER_INTERFACES_H

#include "ILowerLayer.h"

namespace ssp21
{
    /**
     * dual layers are both lower/upper and can be bound to a lower/upper
     */
    class IDualLayer : public ILowerLayer, public IUpperLayer
    {
    public:

        virtual ~IDualLayer() {}

        virtual void bind(ILowerLayer& lower, IUpperLayer& upper) = 0;
    };

}

#endif