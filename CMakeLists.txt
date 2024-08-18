cmake_minimum_required(VERSION 3.10)
project(qwistys_lib C)

set(CMAKE_C_STANDARD 99)

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Optionally enable telemetry
option(ENABLE_QWISTYS_TELEMETRY "Enable telemetry for qwistys_lib" OFF)

# Add include directories
include_directories(inc)

# Create the test executable
add_executable(test_qwistys_lib test_qwistys_lib.c)

# Optionally define ENABLE_QWISTYS_TELEMETRY
if(ENABLE_QWISTYS_TELEMETRY)
    target_compile_definitions(test_qwistys_lib PRIVATE ENABLE_QWISTYS_TELEMETRY)
endif()

# Link libraries if necessary (in this case, none are required)
