cmake_minimum_required(VERSION 3.05)

#set_property(GLOBAL PROPERTY DEVICE_TYPE X64 ARM7 ARM8)
#if ("${DEVICE_TYPE}" MATCHES "^(X64|ARM7|ARM8)$")
#    message(STATUS "DEVICE_TYPE: ${DEVICE_TYPE}")
#else()
#    message(FATAL_ERROR "Unsupported DEVICE_TYPE: ${DEVICE_TYPE}")
#endif()

#if ("${DEVICE_TYPE}" MATCHES "^(ARM7)$")
#    message(STATUS "Configure Compiler For: ${DEVICE_TYPE}")
#    set(CMAKE_C_COMPILER "/usr/bin/arm-linux-gnueabihf-gcc")
#    set(CMAKE_CXX_COMPILER "/usr/bin/arm-linux-gnueabihf-g++")
#elseif ("${DEVICE_TYPE}" MATCHES "^(ARM8)$")
#    message(STATUS "Configure Compiler For: ${DEVICE_TYPE}")
#    set(CMAKE_C_COMPILER "/usr/bin/aarch64-linux-gnu-gcc")
#    set(CMAKE_CXX_COMPILER "/usr/bin/aarch64-linux-gnu-g++")
#endif()

project(mnist_module)
## Set current version and minimal executable version.
set(MAJOR_VERSION 1)
set(MISOR_VERSION 0)
set(REVISION_VERSION 2)

set(MNIST_VERSION ${MAJOR_VERSION}.${MISOR_VERSION}.${REVISION_VERSION})

add_definitions(-DMNIST_VERSION=\"${MNIST_VERSION}\")
message(STATUS "Software Version: ${MNIST_VERSION}")

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
if (CMAKE_SYSTEM_PROCESSOR MATCHES x64)
    message(STATUS "link the x64 lib")
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/x64)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES armv8)
    message(STATUS "link the armv8 lib")
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/armv8)
endif()

#if ("${DEVICE_TYPE}" MATCHES "^(ARM8)$")
#    message(STATUS "link the armv8 lib")
#    link_directories(
#        ${CMAKE_CURRENT_SOURCE_DIR}/lib/armv8
#    )
#elseif ("${DEVICE_TYPE}" MATCHES "^(X64)$")
#    message(STATUS "link the x64 lib")
#    link_directories(
#        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64
#    )
#endif()

add_executable(mnist_test main.cc)

target_link_libraries(mnist_test tvm_runtime ${OpenCV_LIBS} pthread dl)
