# This toolchain file assumes that the Debian package g++-arm-linux-gnueabihf was installed via apt
# and that it was installed to its default location: /usr/arm-linux-gnueabihf
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# required to pick up the correct headers/libraries
set(CMAKE_FIND_ROOT_PATH /usr/arm-linux-gnueabihf)

# these are put in /usr/lib when installed via APT so already on the path
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
