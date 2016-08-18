
#include "ssp21/HandshakeData.h"

namespace ssp21
{		
	
	HandshakeData::HandshakeData(
		const KeyPair& local_static_keys,
		const HandshakeAlgorithms& algorithms
		) : algorithms_(algorithms)
	{
		this->local_static_keys_.clone(local_static_keys);
	}

	void HandshakeData::initialize()
	{
		this->clear();
	}
	
	void HandshakeData::clear()
	{
		local_ephemeral_keys_.private_key.clear();		
	}
}


