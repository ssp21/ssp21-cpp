get_filename_component(settings_cmake_dir "${CMAKE_CURRENT_LIST_FILE}" PATH)

if(STATIC_LIBS)
        message("Static libs is: ${STATIC_LIBS}")
	set(LIB_TYPE STATIC)
endif()

if (UNIX)
	if(NOT DEFINED LIB_TYPE)
		set(LIB_TYPE SHARED)
	endif()
	include("${settings_cmake_dir}/inc/posix.cmake")
elseif(WIN32)
	if(NOT DEFINED LIB_TYPE)
		set(LIB_TYPE STATIC)
	endif()
	include("${settings_cmake_dir}/inc/windows.cmake")	
else()
	message(FATAL_ERROR "Your platform is not supported: ${CMAKE_SYSTEM_NAME}")
endif()

include("${settings_cmake_dir}/inc/sodium.cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
