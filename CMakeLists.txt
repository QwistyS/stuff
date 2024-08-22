cmake_minimum_required(VERSION 3.10)
project(qwistys_lib C)

set(CMAKE_C_STANDARD 99)

set(QWISTYS_SOURCES
    inc/qwistys_alloc.c
    inc/qwistys_avltree.c
    inc/qwistys_stack.c
    inc/qwistys_flexa.c
)

add_library(qwistys_lib STATIC ${QWISTYS_SOURCES})

target_include_directories(qwistys_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:include>
)

# Optionally enable telemetry
option(ENABLE_QWISTYS_TELEMETRY "Enable telemetry for qwistys_lib" OFF)
if(ENABLE_QWISTYS_TELEMETRY)
    target_compile_definitions(qwistys_lib PUBLIC ENABLE_QWISTYS_TELEMETRY)
endif()