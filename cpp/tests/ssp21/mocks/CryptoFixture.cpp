
#include "CryptoFixture.h"

#include "MockCryptoBackend.h"

namespace ssp21
{

    CryptoFixture::CryptoFixture()
    {
        assert(MockCryptoBackend::instance);
        MockCryptoBackend::instance->fixture = this;
    }

    CryptoFixture::~CryptoFixture()
    {
        assert(MockCryptoBackend::instance);
        MockCryptoBackend::instance->fixture = nullptr;
    }

    void CryptoFixture::set_fail_dh_x25519(bool fail)
    {
        fail_dh_x25519 = fail;
    }

    bool CryptoFixture::empty_actions() const
    {
        return actions.empty();
    }

    void CryptoFixture::expect(const std::initializer_list<CryptoAction>& expected)
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

    void CryptoFixture::expect_empty()
    {
        this->expect({});
    }

}


