cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 14)

set(TVM_ROOT /Users/rqg/test/tvm/)
include_directories(${TVM_ROOT})
include_directories(${TVM_ROOT}/include)
include_directories(${TVM_ROOT}/3rdparty/dlpack/include)
include_directories(${TVM_ROOT}/3rdparty/dmlc-core/include)
include_directories(${TVM_ROOT}/apps/cpp_rpc)
include_directories(include)

link_directories(libs/${CMAKE_ANDROID_ARCH_ABI})

add_definitions(-DDMLC_USE_LOGGING_LIBRARY=<tvm/runtime/logging.h>)
add_definitions(-DTVM_USE_LIBBACKTRACE=0)
add_definitions(-DTVM_LOG_CUSTOMIZE=1)
add_definitions(-DTVM_GRAPH_EXECUTOR_ARM_COMPUTE_LIB)

add_library(tvm_glue
        SHARED

        main.cpp
        rpc.cc
        runtime.cc
        )

target_link_libraries(
        tvm_glue
        log
        arm_compute
        arm_compute_core
        arm_compute_graph
)