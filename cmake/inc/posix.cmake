set(PTHREAD pthread)   
find_package(Threads)

if(STATICLIBS)
	set(LIB_TYPE STATIC)
else()
	set(LIB_TYPE SHARED)
endif()


include (CheckLibraryExists)
check_library_exists(pthread pthread_timedjoin_np "" HAVE_PHTREAD_TIMEDJOIN_NP)
set(CMAKE_C_FLAGS "-Wall -W -Wno-multichar -Wunused-variable -Wno-unused-parameter -Wunused-function -Wunused -Wno-system-headers -Wwrite-strings")
if (${CYGWIN})
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
elseif(${MINGW})
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
else()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated")

set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-parentheses --std=c++14")

# Compiler-specific detection
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9.3")
        	message(FATAL_ERROR "GCC version 4.9.3 or greater is required")
	endif()	
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else ()
	message(FATAL_ERROR "Your C++ compiler is not supported at this time: ${CMAKE_CXX_COMPILER_ID}")
endif ()

if(COVERAGE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0")
endif()

if (WERROR)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()
