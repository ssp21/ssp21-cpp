#ifndef SSP21_INTEGERFIELD_H
#define SSP21_INTEGERFIELD_H

#include "ssp21/crypto/IMessagePrinter.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/gen/ParseError.h"

namespace ssp21 {

template <typename IntegerType>
class IntegerField final {
    using integer_t = typename IntegerType::type_t;

public:
    size_t size() const
    {
        return IntegerType::size;
    }

    operator integer_t&()
    {
        return value;
    }

    operator integer_t() const
    {
        return value;
    }

    IntegerField()
    {
    }

    IntegerField& operator=(integer_t value)
    {
        this->value = value;
        return *this;
    }

    explicit IntegerField(integer_t value)
        : value(value)
    {
    }

    ParseError read(seq32_t& input)
    {
        return IntegerType::read_from(input, this->value) ? ParseError::ok : ParseError::insufficient_bytes;
    }

    FormatError write(wseq32_t& output) const
    {
        return IntegerType::write_to(output, this->value) ? FormatError::ok : FormatError::insufficient_space;
    }

    void print(const char* name, IMessagePrinter& printer) const
    {
        printer.print(name, this->value);
    }

    integer_t value = 0;
};

}

#endif
