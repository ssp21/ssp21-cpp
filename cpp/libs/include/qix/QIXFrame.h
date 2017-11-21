#ifndef SSP21PROXY_QIXFRAME_H
#define SSP21PROXY_QIXFRAME_H

#include "ssp21/crypto/BufferTypes.h"

#include <memory>

/**
* Data contained in a QIX Frame
*/
class QIXFrame
{
public:

    QIXFrame() = default;

    uint64_t key_id = 0;
    ssp21::SymmetricKey key;
};

#endif
