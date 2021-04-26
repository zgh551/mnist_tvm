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

## dependence

- opencv 

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

