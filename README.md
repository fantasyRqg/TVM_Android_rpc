# TVM Android RPC

## How to build

 - In `TVM_Android_rpc/app/src/main/cpp/CMakeLists.txt`  set `set(TVM_ROOT /Users/rqg/test/tvm)` to `set(TVM_ROOT path_to_your_tvm_base_dir)`
 - Connect your android device to PC, and enable ADB
 - `cd TVM_Android_rpc; ./gradlew installDebug` 
 
 
 ## Error & Solution
 - Failed to find the external codegen tool for relay.ext.arm_compute_lib  
Enable ACL compile on host runtime. set `set(USE_ARM_COMPUTE_LIB ON)` in `config.cmake` when you build host runtime
