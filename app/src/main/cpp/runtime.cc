#include <sys/stat.h>

#include <fstream>


#define USE_SORT 1
#define USE_RANDOM 1

#include "../src/runtime/c_runtime_api.cc"
#include "../src/runtime/cpu_device_api.cc"
#include "../src/runtime/dso_library.cc"
#include "../src/runtime/file_utils.cc"
#include "../src/runtime/graph_executor/graph_executor.cc"
#include "../src/runtime/graph_executor/graph_executor_factory.cc"
#include "../src/runtime/library_module.cc"
#include "../src/runtime/logging.cc"
#include "../src/runtime/module.cc"
#include "../src/runtime/ndarray.cc"
#include "../src/runtime/object.cc"
#include "../src/runtime/profiling.cc"
#include "../src/runtime/registry.cc"
#include "../src/runtime/rpc/rpc_channel.cc"
#include "../src/runtime/rpc/rpc_endpoint.cc"
#include "../src/runtime/rpc/rpc_event_impl.cc"
#include "../src/runtime/rpc/rpc_local_session.cc"
#include "../src/runtime/rpc/rpc_module.cc"
#include "../src/runtime/rpc/rpc_server_env.cc"
#include "../src/runtime/rpc/rpc_session.cc"
#include "../src/runtime/rpc/rpc_socket_impl.cc"
#include "../src/runtime/system_library.cc"
#include "../src/runtime/thread_pool.cc"
#include "../src/runtime/threading_backend.cc"
#include "../src/runtime/workspace_pool.cc"

#ifdef TVM_OPENCL_RUNTIME
#include "../src/runtime/opencl/opencl_device_api.cc"
#include "../src/runtime/opencl/opencl_module.cc"
#endif

#ifdef TVM_VULKAN_RUNTIME
#include "../src/runtime/vulkan/vulkan.cc"
#endif

#ifdef USE_SORT

#include "../src/runtime/contrib/sort/sort.cc"

#endif

#ifdef USE_RANDOM
#include "../src/runtime/contrib/random/random.cc"
#endif

#include "log.h"

#define LOG_TAG "tvm"

namespace tvm {
    namespace runtime {
        namespace detail {
// Override logging mechanism
            void LogFatalImpl(const std::string &file, int lineno, const std::string &message) {
                std::string m = file + ":" + std::to_string(lineno) + ": " + message;
                ALOGE("%s", m.data());
            }

            void LogMessageImpl(const std::string &file, int lineno, const std::string &message) {
                std::string m = file + ":" + std::to_string(lineno) + ": " + message;
                ALOGD("%s", m.data());
            }

        }  // namespace detail
    }  // namespace runtime
}  // namespace tvm