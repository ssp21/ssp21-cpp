#ifndef SSP21_SERIALIZATIONUTILS_H
#define SSP21_SERIALIZATIONUTILS_H

#include <memory>
#include <stdexcept>

#include "openpal/container/Buffer.h"

#include "ssp21/util/StringUtil.h"
#include "ssp21/crypto/gen/FormatError.h"

namespace ssp21
{
    namespace serialize
    {

        template <class T>
        std::unique_ptr<openpal::Buffer> to_buffer(const T& value)
        {
            auto buffer = std::make_unique<openpal::Buffer>(value.size());

            auto dest = buffer->as_wslice();

            const auto err = value.write(dest);
            if (any(err)) throw std::runtime_error(strings::join("Error writing type: ", FormatErrorSpec::to_string(err)));
            if(dest.is_not_empty()) throw std::runtime_error("size/buffer mismatch");

            return buffer;
        }

    }
}

#endif