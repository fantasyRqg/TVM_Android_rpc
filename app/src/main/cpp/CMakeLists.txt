cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 14)

set(TVM_ROOT /Users/rqg/Playground/tvm/)
include_directories(${TVM_ROOT})
include_directories(${TVM_ROOT}/include)
include_directories(${TVM_ROOT}/3rdparty/dlpack/include)
include_directories(${TVM_ROOT}/3rdparty/dmlc-core/include)
include_directories(${TVM_ROOT}/apps/cpp_rpc)


add_library(tvm_glue
        SHARED

        main.cpp
        rpc.cc
        runtime.cc
        )
add_definitions( -DDMLC_USE_LOGGING_LIBRARY=<tvm/runtime/logging.h> )
add_definitions( -DTVM_USE_LIBBACKTRACE=0 )
add_definitions( -DTVM_LOG_CUSTOMIZE=1 )

target_link_libraries(
        tvm_glue
        log
)