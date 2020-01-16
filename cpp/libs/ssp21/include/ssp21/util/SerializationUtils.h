#ifndef SSP21_SERIALIZATIONUTILS_H
#define SSP21_SERIALIZATIONUTILS_H

#include <memory>
#include <stdexcept>

#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/util/SecureDynamicBuffer.h"
#include "ssp21/util/StringUtil.h"

namespace ssp21 {
namespace serialize {
    template <class BufferType, class T>
    std::unique_ptr<BufferType> to_any_buffer(const T& value)
    {
        auto buffer = std::make_unique<BufferType>(value.size());

        auto dest = buffer->as_wslice();

        const auto err = value.write(dest);
        if (any(err))
            throw std::runtime_error(strings::join("Error writing type: ", FormatErrorSpec::to_string(err)));
        if (dest.is_not_empty())
            throw std::runtime_error("size/buffer mismatch");

        return buffer;
    }

    template <class T>
    std::unique_ptr<ser4cpp::Buffer> to_buffer(const T& value)
    {
        return to_any_buffer<ser4cpp::Buffer>(value);
    }

    template <class T>
    std::unique_ptr<SecureDynamicBuffer> to_secure_buffer(const T& value)
    {
        return to_any_buffer<SecureDynamicBuffer>(value);
    }
}
}

#endif
