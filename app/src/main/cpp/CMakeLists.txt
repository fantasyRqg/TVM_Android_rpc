cmake_minimum_required(VERSION 3.19)

project(tvm_glue)

set(CMAKE_CXX_STANDARD 17)

set(TVM_ROOT /Users/rqg/Playground/tvm)
include_directories(${TVM_ROOT})
include_directories(${TVM_ROOT}/include)
include_directories(${TVM_ROOT}/3rdparty/dlpack/include)
include_directories(${TVM_ROOT}/3rdparty/dmlc-core/include)
include_directories(${TVM_ROOT}/apps/cpp_rpc)

add_definitions(-DDMLC_USE_LOGGING_LIBRARY=<tvm/runtime/logging.h>)
add_definitions(-DTVM_USE_LIBBACKTRACE=0)
add_definitions(-DTVM_LOG_CUSTOMIZE=1)

add_library(tvm_glue
        SHARED

        main.cpp
        rpc.cc
        )

target_link_libraries(
        tvm_glue
        log
)
