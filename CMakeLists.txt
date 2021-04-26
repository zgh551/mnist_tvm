cmake_minimum_required(VERSION 3.05)
# set(CMAKE_C_COMPILER "/usr/bin/arm-linux-gnueabihf-gcc")
# set(CMAKE_CXX_COMPILER "/usr/bin/arm-linux-gnueabihf-g++")
project(mnist_module_test)
## Set current version and minimal executable version.
set(MAJOR_VERSION 1)
set(MISOR_VERSION 0)
set(REVISION_VERSION 0)

set(MNIST_VERSION ${MAJOR_VERSION}.${MISOR_VERSION}.${REVISION_VERSION})

add_definitions(-DMNIST_VERSION=\"${MNIST_VERSION}\")
message(STATUS ${MNIST_VERSION})

find_package(OpenCV REQUIRED)

##set(TVM_HOME "/home/zgh/Workspace/github/tvm")
include_directories(
    $ENV{TVM_HOME}/3rdparty/dlpack/include
    $ENV{TVM_HOME}/3rdparty/dmlc-core/include
    $ENV{TVM_HOME}/include
    ${OpenCV_INCLUDE_DIRS}
)
message(STATUS ${OpenCV_INCLUDE_DIRS})
#-Wall -Werror
add_compile_options(-std=c++14 -O2)

## set the link lib location
## link the dynamic lib
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)


add_executable(mnist_test main.cc)

target_link_libraries(mnist_test tvm_runtime ${OpenCV_LIBS} pthread dl)
