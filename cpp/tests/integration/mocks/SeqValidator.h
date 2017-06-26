#ifndef SSP21_SEQVALIDATOR_H
#define SSP21_SEQVALIDATOR_H

#include "UpperLayer.h"

#include "testlib/MockExecutor.h"

#include <deque>
#include <memory>

namespace ssp21
{
    class SeqValidator final : public IReceiveValidator
    {
    public:

        virtual void validate(const seq32_t& data) override
        {
            if (this->expected_seqs.empty())
            {
                throw std::logic_error("not expecting data!");
            }

            const auto expected = expected_seqs.front();
            expected_seqs.pop_front();

            if (!data.equals(expected))
            {
                throw std::logic_error("bad comparison!");
            }
        }

        void expect(const seq32_t& data)
        {
            this->expected_seqs.push_back(data);
        }

        bool is_empty() const
        {
            return expected_seqs.empty();
        }

        static std::shared_ptr<SeqValidator> create()
        {
            return std::make_shared<SeqValidator>();
        }

    private:

        std::deque<seq32_t> expected_seqs;
    };

}

#endif
