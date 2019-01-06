#ifndef SSP21PROXY_QIXFRAMEREADER_H
#define SSP21PROXY_QIXFRAMEREADER_H

#include <string>
#include <thread>

#include "openpal/logging/Logger.h"
#include "qix/IQIXFrameHandler.h"

#include <asio.hpp>

/**
* A base class source and look-up for
*/
class QIXFrameReader
{
public:

    QIXFrameReader(const std::shared_ptr<IQIXFrameHandler>& handler, const openpal::Logger& logger, const std::string& serial_device);

    ~QIXFrameReader();


private:

    const std::shared_ptr<IQIXFrameHandler> handler;
    openpal::Logger logger;

    bool shutting_down = false;
    asio::io_service service;
    asio::serial_port port;
    std::unique_ptr<std::thread> thread;

    void run();

    void run_and_catch();

};

#endif
