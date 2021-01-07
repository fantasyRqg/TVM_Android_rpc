cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 14)

set(TVM_ROOT /Users/rqg/test/tvm)
include_directories(${TVM_ROOT})
include_directories(${TVM_ROOT}/include)
include_directories(${TVM_ROOT}/3rdparty/dlpack/include)
include_directories(${TVM_ROOT}/3rdparty/dmlc-core/include)


add_library(tvm_glue
        SHARED

        main.cpp
        rpc_env.cc
        rpc_server.cc
        runtime.cc
        )
add_definitions( -DDMLC_LOG_CUSTOMIZE=1 )
add_definitions( -DDMLC_LOG_BEFORE_THROW=1 )

target_link_libraries(
        tvm_glue
        log
)