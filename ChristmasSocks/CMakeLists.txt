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
set(TCP_BUFFER_SIZE 1024)
set(TCP_PORT 8080)
set(MAX_NUM_CONNECTIONS_QUEUE 5)
set(POLL_TIMEOUT_MSEC 1000) # can set to -1 for no timeout

message(STATUS "This node will listen on TCP port ${TCP_PORT}!")
message(STATUS "The TCP buffer size is ${TCP_BUFFER_SIZE} bytes!")
message(STATUS "The maximum number of connections to put in queue is ${MAX_NUM_CONNECTIONS_QUEUE}")

add_compile_definitions(DEBUG_BUILD=1)
add_compile_definitions(NAME_PROJECT="ChristmasSocks")
add_compile_definitions(MAINTAINER="David Weber")
add_compile_definitions(COMPILE_DATE=${COMPILE_DATE})
add_compile_definitions(TCP_BUFFER_SIZE=${TCP_BUFFER_SIZE})
add_compile_definitions(TCP_PORT=${TCP_PORT})
add_compile_definitions(MAX_NUM_CONNECTIONS_QUEUE=${MAX_NUM_CONNECTIONS_QUEUE})

add_executable(test ${SOURCE_FILES})
