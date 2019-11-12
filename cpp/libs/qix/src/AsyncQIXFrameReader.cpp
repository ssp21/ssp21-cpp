
#include "qix/AsyncQIXFrameReader.h"

#include "qix/QIXFrameParser.h"

#include <log4cpp/LogMacros.h>

#include <cinttypes>

namespace qix {
	namespace async {

		struct State
		{
			State(
				const std::shared_ptr<IQIXFrameHandler>& handler,
				const log4cpp::Logger& logger,
				const std::shared_ptr<exe4cpp::BasicExecutor>& executor
			) :
				handler(handler),
				logger(logger),
				port(*executor->get_service()),
				executor(executor),
				parser(logger)
			{
			
			}

			const std::shared_ptr<IQIXFrameHandler> handler;
			log4cpp::Logger logger;
			asio::serial_port port;
			std::shared_ptr<exe4cpp::BasicExecutor> executor;
			QIXFrameParser parser;
		};

		void read_some(ssp21::wseq32_t dest, const std::shared_ptr<State>& state)
		{									
			const auto callback = [dest, state](std::error_code ec, size_t num_read) {
				
				if (ec) {
					FORMAT_LOG_BLOCK(state->logger, log4cpp::levels::error, "Error reading QIX data from port: %s", ec.message().c_str());
					return;
				}
				
				auto copy = dest.skip(num_read);
				if (copy.is_not_empty())
				{
					// if there's still more to read, continue reading
					state->executor->post([copy, state]() { read_some(copy, state); });
				}
				else
				{
					// otherwise, try to parse
					QIXFrame frame;
					if (state->parser.parse(frame))
					{
						switch (frame.status)
						{
						case(QIXFrame::Status::ok):
							state->handler->handle(frame);
							break;
						case(QIXFrame::Status::key_compromised):
							FORMAT_LOG_BLOCK(state->logger, log4cpp::levels::warn, "Key compromised w/ id: %" PRIu64, frame.key_id);
							break;
						default:
							FORMAT_LOG_BLOCK(state->logger, log4cpp::levels::warn, "Received undefined key status w/ id: %" PRIu64, frame.key_id);
							break;
						}
					}

					// go back to reading from the beginning
					state->executor->post([=]() { read_some(state->parser.get_write_slice(), state); });
				}
            };

			state->port.async_read_some(asio::buffer(dest, dest.length()), callback);
		}
		
		void start_frame_reader(
			const std::shared_ptr<IQIXFrameHandler>& handler,
			const log4cpp::Logger& logger,
			const std::shared_ptr<exe4cpp::BasicExecutor>& executor,
			const SerialSettings& settings
		)
		{
			// we use a shared_ptr<> so we can keep this state alive until the last
			// asynchronous operation completes
			const auto state = std::make_shared<State>(handler, logger, executor);

			// configure the port, any of these synchronous operation can throw
			state->port.open(settings.serial_device);
			state->port.set_option(asio::serial_port_base::baud_rate(settings.baud));
			state->port.set_option(asio::serial_port_base::character_size(settings.data_bits));
			state->port.set_option(asio::serial_port::flow_control(settings.flow_control));
			state->port.set_option(asio::serial_port_base::stop_bits(settings.stop_bits));

			FORMAT_LOG_BLOCK(state->logger, log4cpp::levels::info, "port open for reading QIX frames on: %s", settings.serial_device.c_str());

			// if we get to here, we queue the first read operation onto the executor
			state->executor->post([=]() { read_some(state->parser.get_write_slice(), state); });
		}

	}
}

/*
	
*/