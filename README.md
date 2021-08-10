# Example one
This example introduce one way to use `tvm rpc server` to deploy the **mnist** model  on `x86_64` , `aarch64`or `opencl` device.

## 1. Device
### 1.1. Dependent library
- **TVM Runtime**
1. libtvm-runtime.so

In the `tvm` folder, run `./build.sh deploy` will generate the `tvm_runtime_deploy.run` file in `build_aarch64` folder,  if target device don't contain this dynamic library, you should copy this file to target device and execute command as follow:

```shell
$ chmod 777 tvm_runtime_deploy.run
$ ./tvm_runtime_deploy.run
```

This will auto deploy the `libtvm-runtime.so` to `/usr/sdrv/tvm` folder.

### 1.2. Python library

In device side,check whether install `psutil` and `cloudpickle` package.

```shell
$ python3 -m pip list | grep psutil
$ python3 -m pip list | grep cloudpickle
```

If these package exist, then ignore next step, otherwise install these package.

```shell
$ python3 -m pip install psutil cloudpickle
```

### 1.3 RPC Server

Establish the **RPC** server on target device.

```shell
$ python3 -m tvm.exec.rpc_server --host 0.0.0.0 --port 9090
```

if rpc server is successfully established, it will show message as follow:

```
INFO:RPCServer:bind to 0.0.0.0:9090
```

## 2. Host Build

1. running `build.sh` script for model build.

```shell
$ ./build.sh
```

2. select the target device type

```bash
$ ./build.sh
---| number | target type|
-->   [1]   |   x86_64   |
-->   [2]   |   aarch64  |
-->   [3]   |   opencl   |
-->   [4]   |   all      |
Enter target type number: 1
```
If the enter number is `1` ,the script will deploy model on `x86_64` device.

### 2.3 Result

```shell
[21:57:32] main.cc:42: [mnist tvm]:Image Path: ./5.png                                       
[21:57:32] main.cc:43: [mnist tvm]:Dynamic Lib Path: ./mnist.so
[21:57:32] main.cc:44: [mnist tvm]:Parameter Path: ./mnist.params
[21:57:32] main.cc:45: [mnist tvm]:Soft Version: V1.1.2
[21:57:32] main.cc:58: [mnist tvm]:---Load Image--
[21:57:32] main.cc:59: [mnist tvm]:Image size: 28 X 28
[21:57:32] main.cc:79: [mnist tvm]:--- Device Type Configure: CPU ---
[21:57:32] main.cc:92: [mnist tvm]:---Load Dynamic Lib--
[21:57:32] main.cc:98: [mnist tvm]:---Load Parameters--
[21:57:32] main.cc:130: [mnist tvm]:---Executor[0] Time(set_input):2[us]
[21:57:32] main.cc:131: [mnist tvm]:---Executor[0] Time(run):154[us]
[21:57:32] main.cc:132: [mnist tvm]:---Executor[0] Time(get_output):2[us]
[21:57:32] main.cc:130: [mnist tvm]:---Executor[1] Time(set_input):1[us]
[21:57:32] main.cc:131: [mnist tvm]:---Executor[1] Time(run):132[us]
[21:57:32] main.cc:132: [mnist tvm]:---Executor[1] Time(get_output):1[us]
[21:57:32] main.cc:130: [mnist tvm]:---Executor[2] Time(set_input):2[us]
[21:57:32] main.cc:131: [mnist tvm]:---Executor[2] Time(run):92[us]
[21:57:32] main.cc:132: [mnist tvm]:---Executor[2] Time(get_output):0[us]
[21:57:32] main.cc:130: [mnist tvm]:---Executor[3] Time(set_input):1[us]
[21:57:32] main.cc:131: [mnist tvm]:---Executor[3] Time(run):92[us]
[21:57:32] main.cc:132: [mnist tvm]:---Executor[3] Time(get_output):0[us]
[21:57:32] main.cc:130: [mnist tvm]:---Executor[4] Time(set_input):1[us]
[21:57:32] main.cc:131: [mnist tvm]:---Executor[4] Time(run):98[us]
[21:57:32] main.cc:132: [mnist tvm]:---Executor[4] Time(get_output):0[us]
[21:57:32] main.cc:137: [0]: -2702.9
[21:57:32] main.cc:137: [1]: -2044.03
[21:57:32] main.cc:137: [2]: -1144.48
[21:57:32] main.cc:137: [3]: 3431.76
[21:57:32] main.cc:137: [4]: -2802.41
[21:57:32] main.cc:137: [5]: 4449.7
[21:57:32] main.cc:137: [6]: -2858.96
[21:57:32] main.cc:137: [7]: 127.987
[21:57:32] main.cc:137: [8]: 912.245
[21:57:32] main.cc:137: [9]: 528.989
```

