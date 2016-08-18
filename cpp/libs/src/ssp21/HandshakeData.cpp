
#include "ssp21/HandshakeData.h"

namespace ssp21
{		
	
	HandshakeData::HandshakeData(
		const KeyPair& local_static_keys,
		const HandshakeAlgorithms& algorithms
		) : algorithms_(algorithms)
	{
		local_static_keys_.clone(local_static_keys);
	}

	void HandshakeData::initialize()
	{
		clear();
		algorithms_.gen_keypair(local_ephemeral_keys_);
	}
	
	void HandshakeData::clear()
	{
		handshake_hash_.clear();
		local_ephemeral_keys_.private_key.clear();		
	}

	void HandshakeData::set_hash(const openpal::RSlice& input)
	{		
		algorithms_.hash({ input }, handshake_hash_);
	}
	
	void HandshakeData::mix_hash(const openpal::RSlice& input)
	{
		algorithms_.hash(
			{ handshake_hash_.as_slice(), input },
			handshake_hash_
		);
	}
}


