cmake_minimum_required(VERSION 3.8.1)
project(ChristmasSocks)

# Maybe it works on Windows... I have no idea
if (UNIX)
    message(STATUS "Detecting *nix platform. Proceeding.")
else()
    message(FATAL_ERROR "Cannot compile on non-*nix platform!")
endif(UNIX)

# Get the compile date
string(TIMESTAMP COMPILE_DATE "%Y%m%d")

# Use C++ 17 or later
set(CMAKE_CXX_STANDARD 17)

# All header files must be placed under include/
include_directories("include")

# All source files must be placed under src/
file(GLOB SOURCE_FILES "src/*.cpp")

# Set all macro definitions
set(MAX_EPOLL_EVENTS 10)

add_compile_definitions(NAME_PROJECT="ChristmasSocks")
add_compile_definitions(MAINTAINER="David Weber")
add_compile_definitions(COMPILE_DATE=${COMPILE_DATE})
add_compile_definitions(MAX_EPOLL_EVENTS=${MAX_EPOLL_EVENTS})

# This is technically the default - can set to something else
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set binary name
set(OUTPUT_NAME socks)

add_executable(socks ${SOURCE_FILES})
