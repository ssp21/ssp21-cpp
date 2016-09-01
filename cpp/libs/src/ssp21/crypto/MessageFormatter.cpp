
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

    template <class Spec>
    FormatError write_enum(WSlice& dest, typename Spec::enum_type_t value)
    {
        return UInt8::write_to(dest, Spec::to_type(value)) ? FormatError::ok : FormatError::insufficient_space;
    }

    // enums
    FormatError MessageFormatter::write(openpal::WSlice& dest, Function value)
    {
        return write_enum<FunctionSpec>(dest, value);
    }

    FormatError MessageFormatter::write(openpal::WSlice& dest, CertificateMode value)
    {
        return write_enum<CertificateModeSpec>(dest, value);
    }

    FormatError MessageFormatter::write(openpal::WSlice& dest, DHMode value)
    {
        return write_enum<DHModeSpec>(dest, value);
    }

    FormatError MessageFormatter::write(openpal::WSlice& dest, HandshakeError value)
    {
        return write_enum<HandshakeErrorSpec>(dest, value);
    }

    FormatError MessageFormatter::write(openpal::WSlice& dest, NonceMode value)
    {
        return write_enum<NonceModeSpec>(dest, value);
    }

    FormatError MessageFormatter::write(openpal::WSlice& dest, SessionMode value)
    {
        return write_enum<SessionModeSpec>(dest, value);
    }

    FormatError MessageFormatter::write(openpal::WSlice& dest, HashMode value)
    {
        return write_enum<HashModeSpec>(dest, value);
    }
   
}

