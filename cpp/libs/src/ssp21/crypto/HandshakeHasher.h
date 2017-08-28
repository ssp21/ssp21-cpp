
#ifndef SSP21_HANDSHAKEHASHER_H
#define SSP21_HANDSHAKEHASHER_H

#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/CryptoTypedefs.h"
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

class HandshakeHasher : public openpal::Uncopyable
{

public:

	seq32_t compute(hash_func_t hash, const seq32_t first, const seq32_t second);

private:

	HashOutput value;

};



}

#endif
