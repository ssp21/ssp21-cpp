
#ifndef SSP21_STRINGUTIL_H
#define SSP21_STRINGUTIL_H

#include <sstream>

namespace ssp21
{
    namespace strings
    {
        template <class T, class... Args>
        void add_to_stream(std::ostream& stream, const T& t, Args... args)
        {
            stream << t;
            add_to_stream(stream, args...);
        }

        template <class T, class... Args>
        std::string join(const T& t, Args... args)
        {
            std::ostringstream oss;
            add_to_stream(oss, t, args...);
            return oss.str();
        }

        static void add_to_stream(std::ostream& stream) {}
    }
}

#endif
