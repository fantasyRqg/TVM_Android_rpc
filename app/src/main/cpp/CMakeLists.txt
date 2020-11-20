cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 14)

include_directories(/Users/rqg/test/tvm/include)
include_directories(/Users/rqg/test/tvm)
include_directories(/Users/rqg/test/tvm/3rdparty/dlpack/include)
include_directories(/Users/rqg/test/tvm/3rdparty/dmlc-core/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/${ANDROID_ABI}/)

add_library(tvm_glue
        SHARED

        main.cpp
        rpc_env.cc
        rpc_server.cc
        )
add_definitions( -DDMLC_LOG_CUSTOMIZE=1 )
add_definitions( -DDMLC_LOG_BEFORE_THROW=1 )

target_link_libraries(
        tvm_glue
        log
        tvm_runtime
)