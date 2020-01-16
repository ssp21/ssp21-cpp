#ifndef SSP21PROXY_QIXFRAMERWRITER_H
#define SSP21PROXY_QIXFRAMERWRITER_H

#include <string>

#include <asio.hpp>

#include "QIXFrame.h"

class QIXFrameWriter {
public:
    QIXFrameWriter(const std::string& serial_device);

    ~QIXFrameWriter();

    void write(const QIXFrame& frame);

private:
    asio::io_service service;
    asio::serial_port port;
};

#endif
