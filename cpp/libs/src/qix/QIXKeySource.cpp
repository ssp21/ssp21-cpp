
#include "qix/QIXKeySource.h"
#include "qix/QIXFrameParser.h"

QIXKeySource::QIXKeySource(const std::shared_ptr<IQIXFrameHandler>& handler, const openpal::Logger& logger, const std::string& serial_device) :
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
	
	// launch the thread to read frames
	thread = std::make_unique<std::thread>([this]() { this->run(); });
}

QIXKeySource::~QIXKeySource()
{
	// closing the port causes the port read operation to throw
	port.close();
	thread->join();
}

void QIXKeySource::run()
{
	QIXFrameParser parser(this->logger);

	while (true) {
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
			this->handler->handle(frame);
		}
	}
}