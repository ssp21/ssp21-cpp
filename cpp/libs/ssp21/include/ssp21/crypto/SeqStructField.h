
#ifndef SSP21_SEQSTRUCTFIELD_H
#define SSP21_SEQSTRUCTFIELD_H

#include "openpal/logging/Logger.h"
#include "openpal/logging/LogMacros.h"

#include "ssp21/crypto/IMessagePrinter.h"
#include "ssp21/crypto/VLength.h"

#include "ssp21/util/ICollection.h"

namespace ssp21
{
    template <class StructType, uint32_t MAX_COUNT>
    class SeqStructField : public ICollection<StructType>
    {

    public:

        uint32_t capacity() const
        {
            return MAX_COUNT;
        }

        size_t size() const
        {
            size_t sum = 0;

            for (uint32_t i = 0; i < this->count(); ++i)
            {
                sum += this->get(i)->size();
            }

            return VLength::size_in_bytes(this->count()) + sum;
        }

        ParseError read(seq32_t& input)
        {
            this->clear();

            uint32_t count;
            auto cerr = VLength::read(count, input);
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
            const auto err = VLength::write(this->count(), output);
            if (any(err)) return err;

            for (uint32_t i = 0; i < this->count_; ++i)
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

        StructType const* get(uint32_t i) const
        {
            if (i >= this->count_)
            {
                return nullptr;
            }

            return &this->items_[i];
        }

        uint32_t count() const
        {
            return this->count_;
        }

    private:

        uint32_t count_ = 0;
        StructType items_[MAX_COUNT];
    };

}

#endif
