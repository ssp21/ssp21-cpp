
#include "ssp21/crypto/MessageFormatter.h"

#include "openpal/serialization/BigEndian.h"

using namespace openpal;

namespace ssp21
{

    template <class T>
    FormatError write_integer(WSlice& dest, typename T::type_t value)
    {
        return T::write_to(dest, value) ? FormatError::ok : FormatError::insufficient_space;
    }

    // integers
    FormatError MessageFormatter::write(openpal::WSlice& dest, uint8_t value)
    {
        return write_integer<UInt8>(dest, value);
    }

    FormatError MessageFormatter::write(openpal::WSlice& dest, uint16_t value)
    {
        return write_integer<UInt16>(dest, value);
    }

    FormatError MessageFormatter::write(openpal::WSlice& dest, uint32_t value)
    {
        return write_integer<UInt32>(dest, value);
    }

}

