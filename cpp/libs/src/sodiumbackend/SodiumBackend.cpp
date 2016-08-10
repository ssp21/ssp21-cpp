
#include "sodiumbackend/SodiumBackend.h"

#include <sodium.h>

namespace openpal
{

	bool SodiumBackend::Initialize()
	{
		return sodium_init() != -1;
	}

}


