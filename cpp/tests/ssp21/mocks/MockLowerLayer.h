#ifndef SSP21_MOCKLOWERLAYER_H
#define SSP21_MOCKLOWERLAYER_H

#include "openpal/util/Uncopyable.h"

#include "ssp21/LayerInterfaces.h"

namespace ssp21
{
	class MockLowerLayer : public ILowerLayer, private openpal::Uncopyable
	{

	public:

		virtual void begin_transmit(const Addresses& addr, const openpal::RSlice& message)
		{
		
		}

		virtual bool read_message(Addresses& addr, openpal::RSlice& message)
		{
			return false;
		}
		

	};

}

#endif