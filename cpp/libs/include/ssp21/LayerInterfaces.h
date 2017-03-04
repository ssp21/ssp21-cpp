
#ifndef SSP21_LAYER_INTERFACES_H
#define SSP21_LAYER_INTERFACES_H

#include "ILowerLayer.h"

namespace ssp21
{
    /**
     * dual layers can provide both lower/uppeer and can be bound to lower/upper
     */
    class IDualLayer
    {
    public:

        virtual ~IDualLayer() {}

		virtual ILowerLayer& get_lower() = 0;
		
		virtual IUpperLayer& get_upper() = 0;
 
        virtual void bind(ILowerLayer& lower, IUpperLayer& upper) = 0;
    };

}

#endif