
#ifndef SSP21_SECUREFILE_H
#define SSP21_SECUREFILE_H

#include "ssp21/util/SequenceTypes.h"
#include "ssp21/crypto/gen/FormatError.h"

#include <openpal/container/Buffer.h>
#include <string>
#include <memory>
#include <functional>


namespace ssp21
{

    class SecureFile
    {
        std::unique_ptr<openpal::Buffer> buffer;

        static const size_t default_maximum_file_size = 4096;

        size_t maximum_file_size;

    public:

        SecureFile(size_t maximum_file_size = default_maximum_file_size) : maximum_file_size(maximum_file_size)
        {}

        ~SecureFile();

        seq32_t read(const std::string& path);

        bool write(const std::string& path, const std::function<FormatError (wseq32_t&)>& writer);

		template <class T>
		static bool write_item(const std::string& path, const T& item)
		{
			SecureFile file;
			auto writer = [&](wseq32_t& dest) -> FormatError { return item.write(dest); };
			return file.write(path, writer);
		}

    private:

        void release();

    };


}

#endif
