

if(WIN32)	
	set(SODIUM_LIB_NAME libsodium)
	# these definitions are required when linking libsodium statically on Windows
	add_definitions(-DSODIUM_STATIC=1 -DSODIUM_EXPORT=)

	if(NOT DEFINED SSP21_SODIUM_DIR)
		MESSAGE( FATAL_ERROR "You must define SSP21_SODIUM_DIR")
	endif()

	if(NOT EXISTS "${SSP21_SODIUM_DIR}/include/sodium.h")
	    MESSAGE( FATAL_ERROR "Can't find <sodium.h>. Check the SSP21_SODIUM_DIR")
	endif()

	include_directories(${SSP21_SODIUM_DIR}/include) 
  
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		set(SODIUM_MSVC_BUILD_TYPE "Release")
	else()
		set(SODIUM_MSVC_BUILD_TYPE "Debug")
	endif()

	if(${MSVC_VERSION} EQUAL 1800)
		set(SODIUM_MSVC_PATH v120)
	elseif(${MSVC_VERSION} EQUAL 1900)
		set(SODIUM_MSVC_PATH v140)
	elseif(${MSVC_VERSION} EQUAL 1910)
		set(SODIUM_MSVC_PATH v141)
	elseif(${MSVC_VERSION} EQUAL 1911)
	    set(SODIUM_MSVC_PATH v141)
	else()
		MESSAGE( FATAL_ERROR "Unknown MSVC version: ${MSVC_VERSION}")
	endif()

	set(SSP21_SODIUM_LIB_DIR "${SSP21_SODIUM_DIR}/Win32/Debug/${SODIUM_MSVC_PATH}/static")

	MESSAGE("Using sodium libraries in ${SSP21_SODIUM_LIB_DIR}")
	link_directories(${SSP21_SODIUM_LIB_DIR})

else()
	if(STATIC_LIBS)
  	    set(SODIUM_LIB_NAME libsodium.a)
        else()
	    set(SODIUM_LIB_NAME sodium)
        endif()
endif()
