# TVM Android RPC

## How to build

 - In `TVM_Android_rpc/app/src/main/cpp/CMakeLists.txt`  set `set(TVM_ROOT /Users/rqg/test/tvm)` to `set(TVM_ROOT path_to_your_tvm_base_dir)`
 - Connect your android device to PC, and enable ADB
 - `cd TVM_Android_rpc; ./gradlew installDebug` 