
#include "ProtocolVersion.h"

#include "ssp21/crypto/Constants.h"

namespace ssp21 {

namespace version {

    Version get()
    {
        return Version(
            consts::crypto::protocol_major_version,
            consts::crypto::protocol_minor_version);
    }

}
}
