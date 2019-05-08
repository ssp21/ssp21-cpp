
#include "qix/QIXFrameWriter.h"

#include <ser4cpp/container/StaticBuffer.h>
#include <ser4cpp/serialization/BigEndian.h>

#include <ssp21/link/CastagnoliCRC32.h>

using namespace ssp21;
using namespace ser4cpp;

QIXFrameWriter::QIXFrameWriter(const std::string& serial_device) :
        service(1),
        port(service)
{
    port.open(serial_device);

    // 9600 bps
    port.set_option(asio::serial_port_base::baud_rate(9600));

    // 8/N/1
    port.set_option(asio::serial_port_base::character_size(8));
    port.set_option(asio::serial_port::flow_control(asio::serial_port::flow_control::type::none));
    port.set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::type::one));
}

QIXFrameWriter::~QIXFrameWriter()
{
    std::error_code ec;
    port.close(ec);
}

void QIXFrameWriter::write(const QIXFrame& frame)
{
    if(frame.key_data.length() != 32) {
        throw std::runtime_error("key data must be 32 bytes in length");
    }

    StaticBuffer<uint32_t, 47> frame_buffer;

    auto dest = frame_buffer.as_wseq();

    UInt8::write_to(dest, 0x5A);
    UInt8::write_to(dest, 0xA5);
    UInt64::write_to(dest, frame.key_id);
    dest.copy_from(frame.key_data);
    UInt8::write_to(dest, 0x01); // status - OK

    // write the CRC
    UInt32::write_to(
            dest,
            // skip the sync bytes, calculate over count(8) + key(32) + status(1) == 41 bytes
            CastagnoliCRC32::calc(frame_buffer.as_seq().skip(2).take(41))
    );

    asio::write(port, asio::buffer(frame_buffer.as_seq(), frame_buffer.length()));
}


