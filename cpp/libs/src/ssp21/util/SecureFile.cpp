
#include "ssp21/util/SecureFile.h"

#include "ssp21/util/StringUtil.h"

namespace ssp21
{

    std::unique_ptr<SecureDynamicBuffer> SecureFile::read(const std::string& path, uint32_t max_file_size)
    {
        std::ifstream stream(path, std::ios::binary | std::ios::ate);

        if (!stream.is_open()) throw std::runtime_error(strings::join("Unable to open file: ", path));

        const std::streamsize size = stream.tellg();

        if (size <= 0)
        {
            throw std::runtime_error("file size must be > 0");
        }

        if (size > max_file_size)
        {
            throw std::runtime_error(strings::join("file size of ", size, " exceeds maximum of ", max_file_size));
        }

        stream.seekg(0, std::ios::beg);
        auto buffer = std::make_unique<SecureDynamicBuffer>(static_cast<uint32_t>(size));

        uint8_t* bytes = buffer->as_wslice();

        if (!stream.read(reinterpret_cast<char*>(bytes), size))
        {
            throw std::runtime_error("Error reading file");
        }

        return std::move(buffer);
    }

}

