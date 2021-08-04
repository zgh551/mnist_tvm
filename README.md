# mnist_tvm
This project using **TVM** to deploy the mnist module on `x86_64` , `aarch64`or `opencl` device.

## 1. Build

### 1.1. Front build(python)
The Purpose of build to generate the `xxx.so`and `xxx.params`file for target device. 

1. Enter into `python` folder

```bash
$ cd python
```
2. According to the device type, configure different parameters
- **x86_64**

```python
target_type = 'X86_64'
```

- **aarch64**

```python
target_type = 'ARMv8'
```

- **OpenCL**

```python
target_type = 'OpenCl'
```

3. Run python script

```bash
$ python model_build.py
```

the model dynamic lib will be in `module` folder.


#### 1.1.1. Model Import
For the compilation for different models can  reference to [Compile Deep Learning Models](https://tvm.apache.org/docs/tutorials/index.html#compile-deep-learning-models).

#### 1.1.2. Target Device 
The key parameters for different target device as follow:

- **x86_64**

```python
target = tvm.target.Target('llvm')
target_host = tvm.target.Target('llvm')
```


- **aarch64**

```python
target = tvm.target.Target("llvm -device=arm_cpu -mtriple=aarch64-linux-gnu -mattr=+neon")
target_host = tvm.target.Target('llvm -device=arm_cpu -mtriple=aarch64-linux-gnu -mattr=+neon')
```

- **opencl**

```python
target = tvm.target.Target("opencl -device=mali")
target_host = tvm.target.Target('llvm -device=arm_cpu -mtriple=aarch64-linux-gnu -mattr=+neon')
```

#### 1.1.3. Export Library

When export the dynamic library need to pay attention to the following:

- `x64`

```python
module_lib.export_library(lib_path)
```


- `aarch64`

```python
module_lib.export_library(lib_path, tvm.contrib.cc.cross_compiler("aarch64-linux-gnu-g++"))
```


- `opencl`

```python
module_lib.export_library(lib_path, tvm.contrib.cc.cross_compiler("aarch64-linux-gnu-g++"))
```




### 1.2. Backed build(C++)
#### 1.2.1. `x86_64` Platform

1. in `mnist_tvm` folder create `build_x64` and enter into it

```shell
$ mkdir build_x86_64
$ cd build_x86_64
```

2. configure build environment

default cmake link the `x86_64` runtime dynamic library.

```shell
$ cmake ..
```

3. build the source file

this step will generate `mnist_test` executable file.

```shell
$ make -j$(nproc)
```


#### 1.2.2. `aarch64 ` Platform

1. install cross-compile for `aarch64`

```shell
$ sudo apt-get update
$ sudo apt-get install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
```

2. in `mnist_tvm` folder create `build_aarch64` and enter into it.

```shell
$ mkdir build_aarch64
$ cd build_aarch64
```

3. configure build environment

```shell
$ cmake -DCMAKE_SYSTEM_NAME=Linux \
		-DCMAKE_SYSTEM_VERSION=1 \
		-DMACHINE_NAME=aarch64 \
		-DCMAKE_C_COMPILER=/usr/bin/aarch64-linux-gnu-gcc \
		-DCMAKE_CXX_COMPILER=/usr/bin/aarch64-linux-gnu-g++ \
		-DCMAKE_FIND_ROOT_PATH=/usr/aarch64-linux-gnu \
		-DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
		-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
		-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=BOTH \
		..
```
3. build the project
```shell
$ make -j$(nproc)
```
#### 1.2.3. `OpenCL` Platform

1. in `mnist_tvm` folder create `build_opencl` and enter into it.

```shell
$ mkdir build_opencl
$ cd build_opencl
```

2. configure build environment

```shell
$ cmake -DCMAKE_SYSTEM_NAME=Linux \
		-DCMAKE_SYSTEM_VERSION=1 \
		-DTARGET_DEVICE_TYPE=OpenCL \
		-DMACHINE_NAME=aarch64 \
		-DCMAKE_C_COMPILER=/usr/bin/aarch64-linux-gnu-gcc \
		-DCMAKE_CXX_COMPILER=/usr/bin/aarch64-linux-gnu-g++ \
		-DCMAKE_FIND_ROOT_PATH=/usr/aarch64-linux-gnu \
		-DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
		-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
		-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=BOTH \
		..
```

3. build the project
```shell
$ make -j$(nproc)
```
## 2. Running

### 2.1. depence
- opencv
1. libopencv_core.so
2. libopencv_imgcodecs.so
3. libopencv_imgproc.so

- tvm
1. libtvm-runtime.so

### 2.2. run
when running the executable file,three parameter need to be input.

```shell
executer [image path] [module dynamic lib path] [module parameter path]
```

