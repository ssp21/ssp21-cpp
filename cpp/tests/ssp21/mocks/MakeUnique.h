#ifndef SSP21_MAKEUNIQUE_H
#define SSP21_MAKEUNIQUE_H

#include <memory>

namespace std
{

#ifdef NO_MAKE_UNIQUE_SUPPORT
	template<typename T, typename... Args>
	std::unique_ptr<T> make_unique(Args&&... args) {
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}
#endif

}



#endif