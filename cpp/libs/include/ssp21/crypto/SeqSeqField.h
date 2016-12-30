
#ifndef SSP21_SEQSEQFIELD_H
#define SSP21_SEQSEQFIELD_H

#include "ssp21/crypto/SeqField.h"
#include "openpal/logging/Logger.h"
#include "openpal/logging/LogMacros.h"

namespace ssp21
{

    template <class OuterType, class InnerType, uint8_t MAX_COUNT>
    class SeqSeqField
    {

    public:

        typedef typename OuterType::type_t count_t;
        typedef openpal::RSeq<typename InnerType::type_t> seq_t;

        ParseError read(seq32_t& input)
        {
            this->clear();

            IntegerField<OuterType> count;

            auto cerr = count.read(input);
            if (any(cerr)) return cerr;

            while (count > 0)
            {
                SeqField<InnerType> slice;
                auto serr = slice.read(input);
                if (any(serr)) return serr;

                if (!this->push(slice))
                {
                    return ParseError::impl_capacity_limit;
                }
                --count;
            }

            return ParseError::ok;
        }

        FormatError write(wseq32_t& output) const
        {
            IntegerField<OuterType> count_field(this->count_);

            auto err = count_field.write(output);
            if (any(err)) return err;

            for (count_t i = 0; i < this->count_; ++i)
            {
                SeqField<InnerType> item(this->items_[i]);
                auto serr = item.write(output);
                if (any(serr)) return serr;
            }

            return FormatError::ok;
        }

        void print(const char* name, IMessagePrinter& printer) const
        {
            char message[openpal::max_log_entry_size];
            SAFE_STRING_FORMAT(message, openpal::max_log_entry_size, "%s (count = %u)", name, this->count());
            printer.print(message);

            for (count_t i = 0; i < this->count_; ++i)
            {
                SAFE_STRING_FORMAT(message, openpal::max_log_entry_size, "#%u", i + 1);
                printer.print(message, this->items_[i]);
            }
        }

        void clear()
        {
            this->count_ = 0;
        }

        bool push(const seq_t& item)
        {
            if (this->count_ == MAX_COUNT)
            {
                return false;
            }

            this->items_[this->count_++] = item;

            return true;
        }

        bool read(count_t i, seq_t& item) const
        {
            if (i >= this->count_)
            {
                return false;
            }

            item = this->items_[i];
            return true;
        }

        count_t count() const
        {
            return this->count_;
        }

    private:

        count_t count_ = 0;
        seq_t items_[MAX_COUNT];
    };

}

#endif
