
#include "sodiumbackend/Initialize.h"

#include <sodium.h>

namespace ssp21
{
	namespace sodium
	{

		bool Initialize()
		{
			return sodium_init() != -1;
		}

	}
}


