cmake_minimum_required(VERSION 3.16)

#
# by default generates project to build release package
#
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

project("udp-broadcast" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable("receiver" src/receiver.cpp)
add_executable("sender" src/sender.cpp)
