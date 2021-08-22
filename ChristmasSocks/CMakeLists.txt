cmake_minimum_required(VERSION 3.8.1)
project(ChristmasSocks)

# UNIX variable is set TRUE for Cygwin and Apple systems
# Unfortunately need to first check if UNIX then exit if Apple or Cygwin
if(UNIX)
    message(STATUS "Detected *nix platform. Proceeding.")
    if (APPLE)
        message(FATAL_ERROR "Detected Apple *nix platform. Cannot proceed!")
    endif(APPLE)
    if (CYGWIN)
        message(FATAL_ERROR "Detected Cygwin *nix platform. Cannot proceed!")
    endif(CYGWIN)
else()
    message(FATAL_ERROR "Cannot compile on non-*nix platform!")
endif(UNIX)
message(STATUS "Detected ${CMAKE_SYSTEM}")

# Get the compile date
string(TIMESTAMP COMPILE_DATE "%Y%m%d")

# Use C++ 17 or later
set(CMAKE_CXX_STANDARD 17)

# Set the build type (if not overridden via -DCMAKE_BUILD_TYPE=)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)

# Print the build type
message(STATUS "The build type will be: ${CMAKE_BUILD_TYPE}")

add_compile_definitions(NAME_PROJECT="ChristmasSocks")
add_compile_definitions(MAINTAINER="David Weber")
add_compile_definitions(COMPILE_DATE=${COMPILE_DATE})
add_compile_definitions(CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}")

# This is technically the default - can set to something else
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set binary name
set(OUTPUT_NAME socks)

# Set full path to binary
set(FULL_PATH_BINARY "${CMAKE_BINARY_DIR}/${OUTPUT_NAME}")
message(STATUS "Compiled binary will be ${FULL_PATH_BINARY}")

include_directories(
    "src/logging"
    "src/frontend"
    "src/server"
    "src/utilities"
)

# Avoid using glob patterns
add_executable(${OUTPUT_NAME}
    "src/main.cpp"
    "src/frontend/header_footer.cpp"
    "src/frontend/help_menu.cpp"
    "src/logging/logger.cpp"
    "src/server/epoll_server.cpp"
    "src/server/incoming_client_primitives.cpp"
    "src/server/server_primitives.cpp"
    "src/server/system_configurations.cpp"
    "src/server/log_system_configs.cpp"
    "src/server/commands.cpp"
    "src/utilities/constants.cpp"
    "src/utilities/utilities.cpp"
    "src/utilities/ipc_signal_registers.cpp"
    "src/utilities/config_file_parser.cpp"
    "src/utilities/get_line_breaks.cpp"
)
