#ifndef SSP21_MOCKCRYPTOBACKEND_H
#define SSP21_MOCKCRYPTOBACKEND_H

#include "ssp21/crypto/ICryptoBackend.h"

#include "openpal/util/Uncopyable.h"

#include <deque>
#include <sstream>

namespace ssp21
{
    enum class CryptoActions
    {
        secure_equals,
        hash_sha256,
        hmac_sha256,
        gen_keypair_x25519,
        dh_x25519
    };

    class MockCryptoBackend : public ICryptoBackend, private openpal::Uncopyable
    {

    public:

        static MockCryptoBackend instance;

        virtual void zero_memory(openpal::WSlice data) override;

        virtual bool secure_equals(const openpal::RSlice& lhs, const openpal::RSlice& rhs)  override;

        virtual void hash_sha256(std::initializer_list<openpal::RSlice> data, SecureBuffer& output) override;

        virtual void hmac_sha256(const openpal::RSlice& key, std::initializer_list<openpal::RSlice> data, SecureBuffer& output) override;

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

        template <typename... Args>
        void expect(CryptoActions expected, const Args& ... args)
        {
            if (actions.empty())
            {
                std::ostringstream oss;
                oss << "no more crypto actions while waiting for: " << static_cast<int>(expected);
                throw std::logic_error(oss.str());
            }

            if (actions.front() != expected)
            {
                std::ostringstream oss;
                oss << "expected " << static_cast<int>(expected) << " but next action was " << static_cast<int>(actions.front());
                throw std::logic_error(oss.str());
            }

            actions.pop_front();
            expect(args ...);
        }

        void expect()
        {
            if (!actions.empty())
            {
                std::ostringstream oss;
                oss << "unexpected additional actions: " << actions.size() << std::endl;
                for (auto& action : actions)
                {
                    oss << static_cast<int>(action) << std::endl;
                }
                throw std::logic_error(oss.str());
            }
        }

    private:

        std::deque<CryptoActions> actions;

        MockCryptoBackend() {}
    };

}

#endif
