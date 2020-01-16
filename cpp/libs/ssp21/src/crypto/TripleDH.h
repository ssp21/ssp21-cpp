
#ifndef SSP21_TRIPLEDH_H
#define SSP21_TRIPLEDH_H

#include "ser4cpp/util/Uncopyable.h"
#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/CryptoTypedefs.h"
#include "ssp21/crypto/StaticKeys.h"

namespace ssp21 {

/**
    * Computes a "triple DH" usign the supplied hash function and keys
    */
class TripleDH : public ser4cpp::Uncopyable {

public:
    class Result {

    public:
        Result(seq32_t dh1, seq32_t dh2, seq32_t dh3)
            : dh1(dh1)
            , dh2(dh2)
            , dh3(dh3)
        {
        }

        Result() {}

        const seq32_t dh1;
        const seq32_t dh2;
        const seq32_t dh3;
    };

    Result compute(
        dh_func_t dh,
        const StaticKeys& static_keys,
        const KeyPair& ephemeral_keys,
        const seq32_t& remote_public_static,
        const seq32_t& remote_public_ephemeral,
        std::error_code& ec);

private:
    DHOutput dh1;
    DHOutput dh2;
    DHOutput dh3;
};

}

#endif
