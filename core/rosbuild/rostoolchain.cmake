# DO NOT EDIT THIS FILE
#
# To set up cross-compilation, create the file
# $(ROS_ROOT)/rostoolchain.cmake.  It gets read first, prior to 
# any of cmake's system tests.

#############################################################
#
# An example for using the gumstix arm-linux toolchain is below.
# Copy these lines to $(ROS_ROOT)/rostoolchain.cmake to try them out.
#
#set(CMAKE_SYSTEM_NAME Linux)
#set(CMAKE_C_COMPILER /opt/arm-linux/bin/arm-linux-gcc)
#set(CMAKE_CXX_COMPILER /opt/arm-linux/bin/arm-linux-g++)
#set(CMAKE_FIND_ROOT_PATH /opt/arm-linux)
# Have to set this one to BOTH, to allow CMake to find rospack
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

#############################################################
# Cross-compilation loader
#
# This file is passed to cmake on the command line via
# -DCMAKE_TOOLCHAIN_FILE.

#############################################################
# look for user's toolchain file in $ROS_ROOT/rostoolchain.cmake
find_file(TOOLCHAINCONFIG
          rostoolchain.cmake
          PATHS ENV ROS_ROOT
          NO_DEFAULT_PATH)
if(TOOLCHAINCONFIG)
  message("including user's toolchain file: ${TOOLCHAINCONFIG}")
  include(${TOOLCHAINCONFIG})
endif(TOOLCHAINCONFIG)
#############################################################
