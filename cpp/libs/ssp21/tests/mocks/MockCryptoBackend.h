#ifndef SSP21_MOCKCRYPTOBACKEND_H
#define SSP21_MOCKCRYPTOBACKEND_H

#include "ssp21/crypto/CryptoBackend.h"

#include <memory>

namespace ssp21 {

class CryptoFixture;

class MockCryptoBackend {

    static CryptoFixture* fixture;

    friend class CryptoFixture;

public:
    static void initialize();

    static CryptoFixture* get_fixture();
};

}

#endif
