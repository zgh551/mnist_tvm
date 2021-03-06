#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#

import numpy as np
import onnx
import os
import glob
from onnx import numpy_helper
import tvm
from tvm import te
import tvm.relay as relay


mnist_model = onnx.load('../mnist/mnist-8.onnx')

input_name = "Input3"
shape_dict = {input_name: (1, 1, 28, 28)}
print(shape_dict)

mod, params = relay.frontend.from_onnx(mnist_model, shape_dict)

#target_type = 'X86_64'
#target_type = 'ARMv8'
target_type = 'OpenCL'

if target_type == 'X86_64':
    target = tvm.target.Target('llvm')
    target_host = tvm.target.Target('llvm')
elif target_type == 'ARMv7':
    target      = tvm.target.arm_cpu("rasp3b")
    target_host = tvm.target.Target("llvm -device=arm_cpu -mtriple=aarch64-linux-gnu -mattr=+neon")
elif target_type == 'ARMv8':
    target      = tvm.target.Target("llvm -device=arm_cpu -mtriple=aarch64-linux-gnu -mattr=+neon")
    target_host = tvm.target.Target("llvm -device=arm_cpu -mtriple=aarch64-linux-gnu -mattr=+neon")
elif target_type == 'OpenCL':
    target      = tvm.target.Target("opencl -device=mali")
    #target      = tvm.target.Target("opencl -device=intel_graphics")
    target_host = tvm.target.Target("llvm -device=arm_cpu -mtriple=aarch64-linux-gnu -mattr=+neon")
                                        
print(target)
print(target_host)

with tvm.transform.PassContext(opt_level=3):
    module_lib = relay.build(mod, target=target, target_host=target_host, params=params, mod_name='mnist')
print(module_lib)

# lib export
if target_type == 'X86_64':
    lib_path = "../module/x86_64/mnist.so"
    param_path = "../module/x86_64/mnist.params"
    module_lib.export_library(lib_path)
elif target_type == 'ARMv7':
    lib_path = "../module/armv7/mnist.so"
    param_path = "../module/armv7/mnist.params"
    module_lib.export_library(lib_path, tvm.contrib.cc.cross_compiler('arm-linux-gnueabihf-g++'))
elif target_type == 'ARMv8':
    lib_path = "../module/armv8/mnist.so"
    param_path = "../module/armv8/mnist.params"
    module_lib.export_library(lib_path, tvm.contrib.cc.cross_compiler('aarch64-linux-gnu-g++'))
elif target_type == 'OpenCL':
    lib_path = "../module/opencl/mnist.so"
    param_path = "../module/opencl/mnist.params"
    module_lib.export_library(lib_path, tvm.contrib.cc.cross_compiler('aarch64-linux-gnu-g++'))


with open(param_path, 'wb') as fo:
    fo.write(relay.save_param_dict(params))
