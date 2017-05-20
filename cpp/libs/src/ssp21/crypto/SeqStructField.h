
#ifndef SSP21_SEQSTRUCTFIELD_H
#define SSP21_SEQSTRUCTFIELD_H

#include "openpal/logging/Logger.h"
#include "openpal/logging/LogMacros.h"

namespace ssp21
{

    template <class CountType, class StructType, uint8_t MAX_COUNT>
    class SeqStructField
    {

    public:

        typedef typename CountType::type_t count_t;

        ParseError read(seq32_t& input)
        {
            this->clear();

            IntegerField<CountType> count;

            auto cerr = count.read(input);
            if (any(cerr)) return cerr;

            while (count > 0)
            {
                StructType item;                ;
                auto serr = item.read(input);
                if (any(serr)) return serr;

                if (!this->push(item))
                {
                    return ParseError::impl_capacity_limit;
                }

                --count;
            }

            return ParseError::ok;
        }

        FormatError write(wseq32_t& output) const
        {
            IntegerField<CountType> count_field(this->count_);

            auto err = count_field.write(output);
            if (any(err)) return err;

            for (count_t i = 0; i < this->count_; ++i)
            {
                auto serr = this->items_[i].write(output);
                if (any(serr)) return serr;
            }

            return FormatError::ok;
        }

        void print(const char* name, IMessagePrinter& printer) const
        {
            char message[openpal::max_log_entry_size];
            SAFE_STRING_FORMAT(message, openpal::max_log_entry_size, "%s (count = %u)", name, this->count());
            printer.print(message);

            for (uint32_t i = 0; i < this->count_; ++i)
            {
                SAFE_STRING_FORMAT(message, openpal::max_log_entry_size, "field #%u", i + 1);
                printer.print(message);

                this->items_[i].print("test", printer); // TODO
            }
        }

        void clear()
        {
            this->count_ = 0;
        }

        bool push(const StructType& item)
        {
            if (this->count_ == MAX_COUNT)
            {
                return false;
            }

            this->items_[this->count_++] = item;

            return true;
        }

        bool read(count_t i, StructType& item) const
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
        StructType items_[MAX_COUNT];
    };

}

#endif
