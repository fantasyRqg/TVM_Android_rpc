
m numpy.core.fromnumeric import shape
import torch
from torch._C import device
import torchvision
import numpy as np
import time
from torch._C import device
import tvm
from tvm import relay, autotvm, relay
import tvm.relay.testing
from tvm.contrib.utils import tempdir
from tvm.contrib import graph_executor

import torch
from IPython import embed
from tvm.relay.op.contrib.arm_compute_lib import partition_for_arm_compute_lib

device_key = "vivo_000009f6f4c48da0"
tracker_ip = '127.0.0.1'
tracker_port = 9117
toolchain = "/data/ndk/android-ndk-r21e/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang"


model_name = 'resnet18'
device = torch.device("cuda")
model = getattr(torchvision.models, model_name)(pretrained=True)
model.to(device)
model = model.eval()
input_shape = [1, 3, 224, 224]
input_data = torch.randn(input_shape).cuda()
scripted_model = torch.jit.trace(model, input_data).eval()

list_img = [np.random.rand(1, 3, 224, 224).astype(np.float32) * (i % 9 + 1) for i in range(10)]
data = []
for img in list_img:
    k = torch.from_numpy(img)
    data.append(k.cuda())

@torch.no_grad()
def test_torch(model, list_img):
    since = time.time()
    list_top, list_tpoint = [], []
    for img_x in list_img:
        img_x = img_x
        output = model(img_x)
        output = output.cpu()
        top1 = np.argmax(output.numpy()[0])
        list_top.append(top1)
        list_tpoint.append(time.time() - since)
    return list_top, list_tpoint
    

def test_tvm(list_img):
    input_name = "input0"
    shape_list = [(input_name, input_shape)]
    mod, params = relay.frontend.from_pytorch(scripted_model, shape_list)

    # use acl
    mod = partition_for_arm_compute_lib(mod)

    arch = "arm64"
    target = tvm.target.Target("llvm -mtriple=%s-linux-android" % arch)
    target = "llvm -device=arm_cpu -model=android -mtriple=aarch64-linux-android -mattr=+neon"
    with tvm.transform.PassContext(opt_level=3):
        lib = relay.build(mod, target=target, params=params)

    tmp = tempdir()
    filename = "resnet.so"
    lib.export_library(tmp.relpath(filename), cc=toolchain)
    # upload module to device
    print("Upload...")
    remote = autotvm.measure.request_remote(device_key, tracker_ip, tracker_port, timeout=100)
    print(tmp.relpath(filename))
    remote.upload(tmp.relpath(filename))
    rlib = remote.load_module(filename)
    ctx = remote.device(str(target), 0)
    print(ctx)
    module = graph_executor.GraphModule(rlib["default"](ctx))
    since = time.time()
    list_top, list_tpoint = [], []
    for img_x in list_img:
        raw_data = img_x.cpu().detach().numpy()
        tvm_input = tvm.nd.array(raw_data)
        module.set_input("input0", tvm_input)
        module.run()
        output = module.get_output(0)
        top1 = np.argmax(output.asnumpy()[0])
        list_top.append(top1)
        list_tpoint.append(time.time() - since)
    return list_top, list_tpoint


torch_top, torch_points = test_torch(scripted_model, data)
tvm_top, tvm_points = test_tvm(data)
print(torch_top == tvm_top)
print(torch_points[-1])


