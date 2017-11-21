#ifndef SSP21PROXY_QIXFRAMEPARSER_H
#define SSP21PROXY_QIXFRAMEPARSER_H

#include "QIXFrame.h"

#include "openpal/logging/Logger.h"

#include "openpal/container/StaticBuffer.h"

/**
* Data contained in a QIX Frame
*/
class QIXFrameParser : openpal::Uncopyable
{
    static const uint8_t sync1 = 0x5A;
    static const uint8_t sync2 = 0xA5;

    // sync + id + key + crc
    static const uint8_t total_frame_size = 2 + 8 + 32 + 4;

public:

    QIXFrameParser(const openpal::Logger& logger) : logger(logger) {}

    // the buffer that needs to be filled with data before calling parse()
    ssp21::wseq32_t get_write_slice();

    // optionally consume some input, remaining input should be preserved
    bool parse(QIXFrame& frame);

private:

    void read_frame_fields(QIXFrame& frame);

    uint32_t calc_frame_crc();

    uint32_t read_frame_crc();

    static bool find_sync(ssp21::seq32_t& input);

    uint32_t num_bytes_buffered = 0;
    openpal::StaticBuffer<uint32_t, total_frame_size> buffer;

    openpal::Logger logger;

};

#endif
