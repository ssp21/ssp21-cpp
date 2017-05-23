
#include "ssp21/util/SecureFile.h"

#include "ssp21/crypto/Crypto.h"

#include <fstream>

namespace ssp21
{

    SecureFile::~SecureFile()
    {
        this->release();
    }

    seq32_t SecureFile::read(const std::string& path)
    {
        this->release();

        std::ifstream stream(path, std::ios::binary | std::ios::ate);

        if (!stream.is_open()) return seq32_t::empty();

        const std::streamsize size = stream.tellg();

        if (size < 0 || size > maximum_file_size)
        {
            return seq32_t::empty();
        }

        stream.seekg(0, std::ios::beg);
        this->buffer = std::make_unique<openpal::Buffer>(static_cast<uint32_t>(size));
        uint8_t* bytes = this->buffer->as_wslice();

        char* dest = reinterpret_cast<char*>(bytes);

        if (stream.read(dest, size))
        {
            return this->buffer->as_rslice();
        }
        else
        {
            return seq32_t::empty();
        }
    }

    bool SecureFile::write(const std::string& path, const std::function<FormatError(wseq32_t&)>& writer)
    {
        this->release();
        this->buffer = std::make_unique<openpal::Buffer>(static_cast<uint32_t>(maximum_file_size));

        auto dest = this->buffer->as_wslice();
        const auto err = writer(dest);

        if (any(err)) return false;

        const auto length = this->buffer->length() - dest.length();
        const uint8_t* written = this->buffer->as_rslice().take(length);
        const char* data = reinterpret_cast<const char*>(written);

        std::ofstream stream(path, std::ios::binary);

        if (!stream.is_open()) return false;

        if (stream.write(data, length))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void SecureFile::release()
    {
        if (this->buffer)
        {
            Crypto::zero_memory(this->buffer->as_wslice());
            this->buffer.reset();
        }
    }

}

