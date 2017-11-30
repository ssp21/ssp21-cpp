# This toolchain file assumes that the Debian package g++-arm-linux-gnueabihf was installed via apt
# and that it was installed to its default location: /usr/arm-linux-gnueabihf
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLS_ROOT /usr/local/arm-linux-gnueabihf-4.9)
set(CMAKE_SYSROOT ${TOOLS_ROOT})

# these are put in /usr/lib when installed via APT so already on the path
set(CMAKE_C_COMPILER ${TOOLS_ROOT}/usr/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${TOOLS_ROOT}/usr/bin/arm-linux-gnueabihf-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
