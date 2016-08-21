
#include "ssp21/MessagePrinter.h"

#include "openpal/logging/LogMacros.h"

#include "openpal/util/ToHex.h"

using namespace openpal;

namespace ssp21 {
	
	void MessagePrinter::print_unsigned(ILinePrinter& printer, const char* name, uint32_t value)
	{
		char buffer[max_line_size];
		SAFE_STRING_FORMAT(buffer, max_line_size, "%s: %u", name, value);
		printer.print(buffer);
	}

	void MessagePrinter::print_hex(ILinePrinter& printer, const openpal::RSlice& data)
	{
		auto copy = data;
		while (copy.is_not_empty())
		{
			copy = print_hex_line(printer, copy);
		}
	}

	openpal::RSlice MessagePrinter::print_hex_line(ILinePrinter& printer, const openpal::RSlice& data)
	{
		char buffer[max_line_size];
				
		uint32_t count = 0;

		while ((count < max_hex_per_line) && (count < data.length())) {
			auto pos = count * 3;
			buffer[pos] = to_hex_char((data[pos] & 0xf0) >> 4);
			buffer[pos + 1] = to_hex_char((data[pos] & 0xf0) >> 4);
			buffer[pos + 2] = ' ';
			++count;
		}

		static_assert((3 * max_hex_per_line) < max_line_size, "bad configuration");

		buffer[3 * count] = '\0';
		return data.skip(count);
	}

	template <int size, class EnumSpec>
	void print_any_enum(ILinePrinter& printer, const char* name, typename EnumSpec::enum_type_t value)
	{
		char buffer[size];
		SAFE_STRING_FORMAT(buffer, size, "%s: %s", name, EnumSpec::to_string(value));
		printer.print(buffer);
	}

	
	// integers
	void MessagePrinter::print(ILinePrinter& printer, const char* name, uint8_t value)
	{
		print_unsigned(printer, name, value);	
	}

	void MessagePrinter::print(ILinePrinter& printer, const char* name, uint16_t value)
	{
		print_unsigned(printer, name, value);
	}
	void MessagePrinter::print(ILinePrinter& printer, const char* name, uint32_t value)
	{
		print_unsigned(printer, name, value);
	}

	// enums
	void MessagePrinter::print(ILinePrinter& printer, const char* name, Function value)
	{
		print_any_enum<max_line_size, FunctionSpec>(printer, name, value);
	}

	void MessagePrinter::print(ILinePrinter& printer, const char* name, CertificateMode value)
	{
		print_any_enum<max_line_size, CertificateModeSpec>(printer, name, value);
	}
	
	void MessagePrinter::print(ILinePrinter& printer, const char* name, DHMode value)
	{
		print_any_enum<max_line_size, DHModeSpec>(printer, name, value);
	}

	void MessagePrinter::print(ILinePrinter& printer, const char* name, HandshakeError value)
	{
		print_any_enum<max_line_size, HandshakeErrorSpec>(printer, name, value);
	}

	void MessagePrinter::print(ILinePrinter& printer, const char* name, NonceMode value)
	{
		print_any_enum<max_line_size, NonceModeSpec>(printer, name, value);
	}

	void MessagePrinter::print(ILinePrinter& printer, const char* name, SessionMode value)
	{
		print_any_enum<max_line_size, SessionModeSpec>(printer, name, value);
	}	

	void MessagePrinter::print(ILinePrinter& printer, const char* name, HashMode value)
	{
		print_any_enum<max_line_size, HashModeSpec>(printer, name, value);
	}

	// sequences
	void MessagePrinter::print(ILinePrinter& printer, const char* name, const Seq8& value)
	{
		char buffer[max_line_size];
		SAFE_STRING_FORMAT(buffer, max_line_size, "%s, length = %u", name, value.length());
		printer.print(buffer);
		print_hex(printer, value);
	}
	
	void MessagePrinter::print(ILinePrinter& printer, const char* name, const Seq16& value)
	{
		char buffer[max_line_size];
		SAFE_STRING_FORMAT(buffer, max_line_size, "%s, length = %u", name, value.length());
		printer.print(buffer);
		print_hex(printer, value);
	}
	
	void MessagePrinter::print(ILinePrinter& printer, const char* name, const Seq8Seq16& value)
	{
		char buffer[max_line_size];
		SAFE_STRING_FORMAT(buffer, max_line_size, "%s, count = %u", name, value.count());
		printer.print(buffer);
		
		for (uint32_t i = 0; i < value.count(); ++i)
		{
			SAFE_STRING_FORMAT(buffer, max_line_size, "# %u:", i);
			printer.print(buffer);

			RSlice entry;
			value.read(i, entry);
			print_hex(printer, entry);
		}
	}
	
}

