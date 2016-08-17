
#include "ssp21/MessageFormatter.h"

#include "openpal/serialization/Serialization.h"

using namespace openpal;

namespace ssp21 {
	
template <class T>
bool write_integer(WSlice& dest, typename T::type_t value)
{
	if (dest.length() < T::size) {
		return false;
	}
	else {
		T::write_to_slice(dest, value);
		return true;
	}
}

// integers
bool MessageFormatter::write(openpal::WSlice& dest, uint8_t value)
{
	return write_integer<UInt8>(dest, value);
}

bool MessageFormatter::write(openpal::WSlice& dest, uint16_t value)
{
	return write_integer<UInt16>(dest, value);
}

bool MessageFormatter::write(openpal::WSlice& dest, uint32_t value)
{
	return write_integer<UInt32>(dest, value);
}

template <class Spec>
bool write_enum(WSlice& dest, typename Spec::enum_type_t value)
{
	if (dest.length() < UInt8::size) {
		return false;
	}
	else {
		UInt8::write_to_slice(dest, Spec::to_type(value));				
		return true;
	}
}

// enums
bool MessageFormatter::write(openpal::WSlice& dest, Function value)
{
	return write_enum<FunctionSpec>(dest, value);
}

bool MessageFormatter::write(openpal::WSlice& dest, CertificateMode value)
{
	return write_enum<CertificateModeSpec>(dest, value);
}

bool MessageFormatter::write(openpal::WSlice& dest, DHMode value)
{
	return write_enum<DHModeSpec>(dest, value);
}

bool MessageFormatter::write(openpal::WSlice& dest, HandshakeError value)
{
	return write_enum<HandshakeErrorSpec>(dest, value);
}

bool MessageFormatter::write(openpal::WSlice& dest, NonceMode value)
{
	return write_enum<NonceModeSpec>(dest, value);
}

bool MessageFormatter::write(openpal::WSlice& dest, SessionMode value)
{
	return write_enum<SessionModeSpec>(dest, value);
}

bool MessageFormatter::write(openpal::WSlice& dest, HashMode value)
{
	return write_enum<HashModeSpec>(dest, value);
}

template <class CountType, class SeqType>
bool write_seq(openpal::WSlice& dest, const SeqType& value)
{
	if (value.length() > CountType::max_value) {
		return false;
	}

	const auto count = static_cast<CountType::type_t>(value.length());

	if (!MessageFormatter::write(dest, count)) return false;

	if (dest.length() < value.length()) return false;

	value.copy_to(dest);

	return true;
}

bool MessageFormatter::write(openpal::WSlice& dest, const Seq8& value)
{	
	return write_seq<UInt8>(dest, value);
}

bool MessageFormatter::write(openpal::WSlice& dest, const Seq16& value)
{
	return write_seq<UInt16>(dest, value);
}

bool MessageFormatter::write(openpal::WSlice& dest, const Seq8Seq16& value)
{	
	if (value.count() > UInt8::max_value) {
		return false;
	}

	const uint8_t count = static_cast<UInt8::type_t>(value.count());

	if (!write(dest, count)) return false;
	
	for (UInt8::type_t i = 0; i < count; ++i) 
	{
		Seq16 slice;
		if (!value.read(i, slice)) return false;
		if (!write(dest, slice)) return false;
	}

	return true;
}

}

