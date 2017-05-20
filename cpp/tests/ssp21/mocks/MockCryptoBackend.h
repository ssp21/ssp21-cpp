#ifndef SSP21_MOCKCRYPTOBACKEND_H
#define SSP21_MOCKCRYPTOBACKEND_H

#include "openpal/util/Uncopyable.h"
#include "../gen/CryptoAction.h"

#include <deque>
#include <sstream>
#include <assert.h>

namespace ssp21
{
    class CryptoFixture
    {

    public:

        static CryptoFixture* instance;

        uint8_t fill_byte = 0xFF;
        bool fail_dh_x25519 = false;
        std::deque<CryptoAction> actions;

        CryptoFixture()
        {
            assert(instance == nullptr);
            instance = this;
        }

        ~CryptoFixture()
        {
            assert(instance != nullptr);
            instance = nullptr;
        }

        void set_fail_dh_x25519(bool fail)
        {
            fail_dh_x25519 = fail;
        }

        bool empty_actions() const
        {
            return actions.empty();
        }

        void expect(const std::initializer_list<CryptoAction>& expected)
        {
            int count = 0;

            for (auto& action : expected)
            {
                if (actions.empty())
                {
                    std::ostringstream oss;
                    oss << "no more crypto actions while waiting for: " << CryptoActionSpec::to_string(action) << " after " << count << " actions";
                    throw std::logic_error(oss.str());
                }

                if(actions.front() != action)
                {
                    std::ostringstream oss;
                    oss << "expected " << CryptoActionSpec::to_string(action) << " but next action was " << CryptoActionSpec::to_string(actions.front());
                    oss << " after " << count << " actions";
                    throw std::logic_error(oss.str());
                }

                actions.pop_front();

                ++count;
            }

            if (!actions.empty())
            {
                std::ostringstream oss;
                oss << "unexpected additional actions: " << actions.size() << std::endl;
                for (auto& action : actions)
                {
                    oss << CryptoActionSpec::to_string(action) << std::endl;
                }
                throw std::logic_error(oss.str());
            }
        }

        void expect_empty()
        {
            this->expect({});
        }
    };

}

#endif
