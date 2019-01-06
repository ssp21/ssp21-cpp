
#include "qix/QIXFrameReader.h"
#include "qix/QIXFrameParser.h"

#include "openpal/logging/LogMacros.h"

#include <cinttypes>

QIXFrameReader::QIXFrameReader(const std::shared_ptr<IQIXFrameHandler>& handler, const openpal::Logger& logger, const std::string& serial_device) :
    handler(handler),
    logger(logger),
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

    FORMAT_LOG_BLOCK(this->logger, openpal::levels::info, "Reading QIX frames on: %s", serial_device.c_str());

    // launch the thread to read frames
    thread = std::make_unique<std::thread>([this]()
    {
        this->run_and_catch();
    });
}

QIXFrameReader::~QIXFrameReader()
{
    // closing the port causes the port read operation to throw
    this->shutting_down = true;
    port.close();
    thread->join();
}

void QIXFrameReader::run_and_catch()
{
    try
    {
        this->run();
    }
    catch (const std::exception& ex)
    {
        if (!shutting_down)
        {
            SIMPLE_LOG_BLOCK(this->logger, openpal::levels::error, ex.what());
        }
    }
}

void QIXFrameReader::run()
{
    QIXFrameParser parser(this->logger);

    while (true)
    {
        auto writable = parser.get_write_slice();

        // loop until all the requested data has been received
        while (writable.is_not_empty())
        {
            const auto received = this->port.read_some(asio::buffer(writable, writable.length()));
            writable.advance(received);
        }

        // we can now try to parse
        QIXFrame frame;
        if (parser.parse(frame))
        {
            switch (frame.status)
            {
            case(QIXFrame::Status::ok):
                this->handler->handle(frame);
                break;
            case(QIXFrame::Status::key_compromised):
                FORMAT_LOG_BLOCK(this->logger, openpal::levels::warn, "Key compromised w/ id: %" PRIu64, frame.key_id);
                break;
            default:
                FORMAT_LOG_BLOCK(this->logger, openpal::levels::warn, "Received undefined key status w/ id: %" PRIu64, frame.key_id);
                break;
            }
        }
    }
}
