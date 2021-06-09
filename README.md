# mnist_tvm
This project using **TVM** to deploy the mnist module

## build
1. first `mkdir build`flolder in root 
2. configure the build enviroment

```shell
$ cmake ..
```
3. build the project
```shell
$ make
```

## Dependence

### OpenCV 

- Compilation Tool

1. GCC 
2. CMAKE

- 3rdparty_lib

1. ffmpeg
2. GTK + 2.x
3. libav : libavcodec-dev libavformat-dev libswscale-dev

- optional package

1. libtbb2 libtbb-dev
2. libjpeg-dev, libpng-dev, libtiff-dev, libjasper-dev, libdc1394-22-dev
3. 

1. install third-party-lib 

```
$ sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libatlas-base-dev gfortran libgtk2.0-dev
```



```shell
$ cmake -D CMAKE_CXX_COMPILER=/usr/bin/arm-linux-gnueabihf-g++ \
		-D CMAKE_C_COMPILER=/usr/bin/arm-linux-gnueabihf-gcc \
		-D CMAKE_BUILD_TYPE=RELEASE \
    	-D CMAKE_INSTALL_PREFIX=/usr/local \
    	-D INSTALL_C_EXAMPLES=ON \
    	-D INSTALL_PYTHON_EXAMPLES=ON \
    	-D OPENCV_GENERATE_PKGCONFIG=ON \
    	-D OPENCV_EXTRA_MODULES_PATH=~/opencv_build/opencv_contrib/modules \
    	-D BUILD_EXAMPLES=ON ..
```

#### Cross Compiler

- GCC install

1. ARM32

```shell
$ sudo apt-get install g++-arm-linux-gnueabihf gcc-arm-linux-gnueabihf
```

2. ARM64

```shell
$ sudo apt-get install g++-aarch64-linux-gnu gcc-aarch64-linux-gnu
```



`hf`: Hard float 

base on the actual processor select whether surport `hf`.



- Using default `cmake`

in the folder `opencv/platforms/linux`, find **aarch64-gnu.toolchain.cmake** and **arm-gnueabi.toolchain.cmake**,

1. ARMv7

```shell
$ cmake -D CMAKE_TOOLCHAIN_FILE="/path/to/opencv/platforms/linux/arm-gnueabi.toolchain.cmake" ..
```

1. ARMv8 

```shell
$ cmake -D CMAKE_TOOLCHAIN_FILE="/path/to/opencv/platforms/linux/aarch64-gnu.toolchain.cmake" ..
```

- System Name

|       |         |      |           |              |           |      |
| ----- | ------- | ---- | --------- | ------------ | --------- | ---- |
| Linux | Android | QNX  | WindowsCE | WindowsPhone | Windows10 |      |

- System Processor

1. arm
2. x86

- compiler error

Disable `BUILD_opencv_freetype=OFF`can solve follow error.

````
/usr/lib/gcc-cross/aarch64-linux-gnu/7/../../../../aarch64-linux-gnu/bin/ld: cannot find -lfreetype
/usr/lib/gcc-cross/aarch64-linux-gnu/7/../../../../aarch64-linux-gnu/bin/ld: cannot find -lharfbuzz
````

or add `freetype `lib using cross compiler tool



