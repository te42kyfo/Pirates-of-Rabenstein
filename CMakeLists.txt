cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0003 NEW)
project(RABENSTEIN)

set(RABENSTEIN_VERSION_MAJOR 0)
set(RABENSTEIN_VERSION_MINOR 1)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "native" CACHE STRING
    "Build type. Avilable types are debug, release and native" FORCE)
endif()

set(CMAKE_CXX_FLAGS "-std=c++0x -pthread")
set(CMAKE_CXX_FLAGS_WARN "-Wnon-virtual-dtor -Wno-long-long -Wcast-align -Wchar-subscripts -Wall -Wextra -Wpointer-arith -Wformat-security -fno-check-new -fno-common -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DQT_NO_DEBUG_OUTPUT -Wl,-s ${CMAKE_CXX_FLAGS_WARN}")
set(CMAKE_CXX_FLAGS_NATIVE "-O3 -DNDEBUG -DQT_NO_DEBUG_OUTPUT -march=native -Wl,-s ${CMAKE_CXX_FLAGS_WARN}")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -fno-inline ${CMAKE_CXX_FLAGS_WARN}")

set(LIBRARY_OUTPUT_PATH
  ${CMAKE_CURRENT_BINARY_DIR}/lib
  CACHE PATH
  "Directory for all libraries" FORCE)

set(EXECUTABLE_OUTPUT_PATH
  ${CMAKE_CURRENT_BINARY_DIR}/bin
  CACHE PATH
  "Directory for all executables" FORCE)

set(RABENSTEIN_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  CACHE PATH
  "Directory for all headers" FORCE)

set(RABENSTEIN_DATA_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/data
  CACHE PATH
  "Directory for pictures and levels" FORCE)

configure_file(
  ${RABENSTEIN_INCLUDE_DIR}/config.hpp.in
  ${RABENSTEIN_INCLUDE_DIR}/config.hpp
  )

add_subdirectory(src)




