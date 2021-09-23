import tvm
from tvm import relay
from tvm.relay.op.contrib.arm_compute_lib import partition_for_arm_compute_lib
from tvm import autotvm
from tvm.contrib.utils import tempdir
from tvm.contrib import graph_executor
from tvm.contrib.debugger import debug_executor
import numpy as np

data_type = "float32"
data_shape = (1, 14, 14, 512)
strides = (2, 2)
padding = (0, 0, 0, 0)
pool_size = (2, 2)
layout = "NHWC"
output_shape = (1, 7, 7, 512)
device_key = 'acl'
tracker_ip = '192.168.31.79'
tracker_port = 9190
toolchain = '/Users/rqg/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/aarch64-linux-android21-clang'

data = relay.var('data', shape=data_shape, dtype=data_type)
out = relay.nn.max_pool2d(data, pool_size=pool_size, strides=strides, layout=layout, padding=padding)
module = tvm.IRModule.from_expr(out)

module = partition_for_arm_compute_lib(module)
target = "llvm -device=arm_cpu -model=android -mtriple=aarch64-linux-android -mattr=+neon"
with tvm.transform.PassContext(opt_level=3, disabled_pass=["AlterOpLayout"]):
    lib = relay.build(module, target=target)

tmp = tempdir()
filename = "net.so"
lib.export_library(tmp.relpath(filename), cc=toolchain)
# upload module to device
print("Upload...")
remote = autotvm.measure.request_remote(device_key, tracker_ip, tracker_port, timeout=1000)
print(tmp.relpath(filename))
remote.upload(tmp.relpath(filename))
rlib = remote.load_module(filename)
ctx = remote.device(str(target), 0)
module = graph_executor.GraphModule(rlib["default"](ctx))
d_data = np.random.uniform(0, 1, data_shape).astype(data_type)
map_inputs = {'data': d_data}
module.set_input(**map_inputs)
module.run()
print(module.get_output(0).asnumpy())
