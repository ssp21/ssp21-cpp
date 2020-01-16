
#include "qix/QIXFrameReader.h"
#include "qix/QIXFrameParser.h"

#include "qix/AsyncQIXFrameReader.h"

QIXFrameReader::QIXFrameReader(const std::shared_ptr<IQIXFrameHandler>& handler, const log4cpp::Logger& logger, const SerialSettings& settings) :
    executor(std::make_shared<exe4cpp::BasicExecutor>(std::make_shared<asio::io_service>()))
{
    // launch an asynchronous reader onto the event loop
    qix::async::start_frame_reader(handler, logger, this->executor, settings);

    // run the event loop from a thread
    thread = std::make_unique<std::thread>([this]()
    {
        this->run_and_catch();
    });
}

QIXFrameReader::~QIXFrameReader()
{
    // stop the io_service and join the thread
    this->executor->get_service()->stop();
    thread->join();
}

void QIXFrameReader::run_and_catch()
{
    try
    {
        this->executor->get_service()->run();
    }
    catch (const std::exception&)
    {

    }
}

