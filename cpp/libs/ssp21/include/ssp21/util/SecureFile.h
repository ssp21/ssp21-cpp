
#ifndef SSP21_SECUREFILE_H
#define SSP21_SECUREFILE_H

#include "ssp21/util/SequenceTypes.h"
#include "crypto/gen/FormatError.h"

#include "ssp21/util/SerializationUtils.h"
#include "ssp21/util/SecureDynamicBuffer.h"

#include <memory>
#include <fstream>

namespace ssp21
{

    class SecureFile : openpal::StaticOnly
    {

    public:

        static std::unique_ptr<SecureDynamicBuffer> read(const std::string& path, uint32_t max_file_size = 4096);

        template <class T>
        static void write(const std::string& path, const T& item)
        {
            auto buffer = serialize::to_secure_buffer(item);
            write(path, buffer->as_wslice());
        }

    private:

        static void write(const std::string& path, const wseq32_t& data)
        {
            std::ofstream stream(path, std::ios::binary);

            if (!stream.is_open()) throw std::runtime_error(strings::join("Unable to open file: ", path));

            const uint8_t* buffer = data;

            if (!stream.write(reinterpret_cast<const char*>(buffer), data.length()))
            {
                throw std::runtime_error(strings::join("Unable to write file: ", path));
            }
        }

    };


}

#endif
