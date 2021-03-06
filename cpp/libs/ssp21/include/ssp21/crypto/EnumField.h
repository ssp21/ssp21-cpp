#ifndef SSP21_ENUMFIELD_H
#define SSP21_ENUMFIELD_H

#include "ssp21/crypto/IMessagePrinter.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/gen/ParseError.h"

#include "ser4cpp/serialization/BigEndian.h"

namespace ssp21 {

template <typename Spec>
class EnumField final {
    using enum_t = typename Spec::enum_type_t;

public:
    size_t size() const
    {
        return 1;
    }

    operator enum_t() const
    {
        return value;
    }

    EnumField()
    {
    }

    explicit EnumField(enum_t value)
        : value(value)
    {
    }

    ParseError read(seq32_t& input)
    {
        uint8_t raw_value;
        if (!ser4cpp::BigEndian::read(input, raw_value))
            return ParseError::insufficient_bytes;

        auto enum_value = Spec::from_type(raw_value);

        if (enum_value == Spec::enum_type_t::undefined) {
            return ParseError::undefined_enum;
        }

        this->value = enum_value;

        return ParseError::ok;
    }

    FormatError write(wseq32_t& output) const
    {
        return ser4cpp::BigEndian::write(output, Spec::to_type(value)) ? FormatError::ok : FormatError::insufficient_space;
    }

    void print(const char* name, IMessagePrinter& printer) const
    {
        printer.print(name, Spec::to_string(this->value));
    }

    enum_t value = enum_t::undefined;
};

}

#endif
