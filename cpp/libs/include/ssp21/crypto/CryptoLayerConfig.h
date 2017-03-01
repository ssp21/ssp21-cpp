
#ifndef SSP21_CRYPTOLAYERCONFIG_H
#define SSP21_CRYPTOLAYERCONFIG_H

#include <cstdint>

#include "ssp21/crypto/Constants.h"

namespace ssp21
{

    struct CryptoLayerConfig
    {
        /// The maximum size of a reassembled message
        uint16_t max_reassembly_size = consts::link::max_config_payload_size;
    };


}

#endif
