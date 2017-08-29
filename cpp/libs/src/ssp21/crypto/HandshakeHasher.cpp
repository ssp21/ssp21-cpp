
#include "HandshakeHasher.h"

namespace ssp21
{

    seq32_t HandshakeHasher::compute(hash_func_t hash, const seq32_t first, const seq32_t second)
    {
        // initialize
        hash({ first }, this->value);

        // mix
        hash({ this->value.as_seq(), second }, this->value);


        return this->value.as_seq();
    }

}


