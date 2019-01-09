
#ifndef SSP21_CRYPTOFIXTURE_H
#define SSP21_CRYPTOFIXTURE_H

#include "gen/CryptoAction.h"

#include <deque>
#include <sstream>
#include <assert.h>

namespace ssp21
{
    class CryptoFixture
    {

    public:

        CryptoFixture();
        ~CryptoFixture();

        uint8_t fill_byte = 0xFF;
        bool fail_dh_x25519 = false;
        std::deque<CryptoAction> actions;

        void set_fail_dh_x25519(bool fail);
        bool empty_actions() const;
        void expect(const std::initializer_list<CryptoAction>& expected);
        void expect_empty();
    };

}

#endif
