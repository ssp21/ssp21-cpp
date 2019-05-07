#ifndef SSP21PROXY_QIXFRAMEPARSER_H
#define SSP21PROXY_QIXFRAMEPARSER_H

#include "QIXFrame.h"

#include "log4cpp/Logger.h"

#include "ser4cpp/container/StaticBuffer.h"

/**
* Data contained in a QIX Frame
*/
class QIXFrameParser : ser4cpp::Uncopyable
{
    static const uint8_t sync1 = 0x5A;
    static const uint8_t sync2 = 0xA5;

    static const uint8_t sync_size = 2;
    static const uint8_t key_id_size = 8;
    static const uint8_t key_size = 32;
    static const uint8_t status_size = 1;
    static const uint8_t crc_size = 4;

    //  id + key + status
    static const uint8_t crc_data_size = key_id_size + key_size + status_size;

    // sync + id + key + status + crc
    static const uint8_t total_frame_size = sync_size + crc_data_size + crc_size;

public:

    inline static uint8_t get_fixed_frame_size()
    {
        return total_frame_size;
    }

    QIXFrameParser(const log4cpp::Logger& logger) : logger(logger) {}

    // the buffer that needs to be filled with data before calling parse()
    ssp21::wseq32_t get_write_slice();

    // optionally consume some input, remaining input should be preserved
    bool parse(QIXFrame& frame);

private:

    static QIXFrame::Status get_status(uint8_t value);

    void read_frame_fields(QIXFrame& frame);

    uint32_t calc_frame_crc();

    uint32_t read_frame_crc();

    static uint32_t find_sync(ssp21::seq32_t& input);

    uint32_t num_bytes_buffered = 0;
    ser4cpp::StaticBuffer<uint32_t, total_frame_size> buffer;

    log4cpp::Logger logger;

};

#endif
