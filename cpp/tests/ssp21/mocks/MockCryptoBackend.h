#ifndef SSP21_MOCKCRYPTOBACKEND_H
#define SSP21_MOCKCRYPTOBACKEND_H

#include "ssp21/crypto/ICryptoBackend.h"

#include "openpal/util/Uncopyable.h"
#include "../gen/CryptoAction.h"

#include <deque>
#include <sstream>

namespace ssp21
{
    class MockCryptoBackend : public ICryptoBackend, private openpal::Uncopyable
    {

    public:

        static MockCryptoBackend instance;

        virtual void zero_memory(openpal::WSlice data) override;

        virtual bool secure_equals(const Seq8& lhs, const Seq8& rhs)  override;

        virtual void hash_sha256(std::initializer_list<openpal::RSlice> data, SecureBuffer& output) override;

        virtual void hmac_sha256(const Seq8& key, std::initializer_list<openpal::RSlice> data, SecureBuffer& output) override;

        virtual void gen_keypair_x25519(KeyPair& pair) override;

        virtual void dh_x25519(const PrivateKey& priv_key, const openpal::RSlice& pub_key, DHOutput& output, std::error_code& ec) override;

        uint8_t fill_byte = 0xFF;

        bool empty_actions() const
        {
            return actions.empty();
        }

        void clear_actions()
        {
            actions.clear();
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

                if (actions.front() != action)
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

    private:

        std::deque<CryptoAction> actions;

        MockCryptoBackend() {}
    };

    struct CryptoTest
    {
        CryptoTest()
        {
            MockCryptoBackend::instance.clear_actions();
        }

        MockCryptoBackend* operator->()
        {
            return &MockCryptoBackend::instance;
        }

        ~CryptoTest()
        {
            MockCryptoBackend::instance.clear_actions();
        }
    };

}

#endif
