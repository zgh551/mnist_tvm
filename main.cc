
#include <dlpack/dlpack.h>
#include <tvm/runtime/module.h>
#include <tvm/runtime/packed_func.h>
#include <tvm/runtime/registry.h>
// opencv 
#include <opencv4/opencv2/opencv.hpp>
#include <cstdio>
#include <fstream>


int main(int argc, char *argv[])
{
    LOG(INFO) << "soft version: v" << MNIST_VERSION;
    LOG(INFO) << "test_mnist_tvm";

    // read the image
    cv::Mat image, gray_image;
    image = cv::imread("../test_dataset/test.png");
    if(image.data == nullptr)
    {
        LOG(INFO) << "image don't exist";
        return 0;
    }
    else{
        cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
        gray_image.convertTo(gray_image, CV_32FC3);
        LOG(INFO) << gray_image;
        // cv::imshow("mnist image", gray_image);
        // cv::waitKey(0);
    }

    // create tensor
    DLTensor *x;
    int input_ndim = 4;
    int64_t input_shape[4] = {1, 1, 28, 28};

    int dtype_code = kDLFloat;
    int dtype_bits = 32;
    int dtype_lanes = 1;
    int device_type = kDLCPU;
    int device_id   = 0;

    TVMArrayAlloc(input_shape, input_ndim, dtype_code, dtype_bits, dtype_lanes, device_type, device_id, &x);

    memcpy(x->data, gray_image.data, 28 * 28 * sizeof(float));

    tvm::runtime::Module mod_dylib = tvm::runtime::Module::LoadFromFile("../lib/mnist.so");
    
    std::ifstream params_in("../lib/mnist.params", std::ios::binary);
    std::string params_data((std::istreambuf_iterator<char>(params_in)), std::istreambuf_iterator<char>());
    params_in.close();

    TVMByteArray params_arr;
    params_arr.data = params_data.c_str();
    params_arr.size = params_data.length();

    DLDevice dev{kDLCPU, 0};
    LOG(INFO) << "mod creating";
    tvm::runtime::Module mod = mod_dylib.GetFunction("mnist")(dev);

    // set input data
    tvm::runtime::PackedFunc set_input = mod.GetFunction("set_input");
    set_input("Input3", x);

    // load parameters
    tvm::runtime::PackedFunc load_params = mod.GetFunction("load_params");
    load_params(params_arr);
    

    DLTensor *y;
    int output_ndim = 2;
    int64_t output_shape[2] = {1, 10};
    TVMArrayAlloc(output_shape, output_ndim, dtype_code, dtype_bits, dtype_lanes, device_type, device_id, &y);

    // run
    tvm::runtime::PackedFunc run = mod.GetFunction("run");

    // get output data
    tvm::runtime::PackedFunc get_output = mod.GetFunction("get_output");
    run();
    get_output(0, y);

    auto result = static_cast<float *>(y->data);
    for (int i = 0; i < 10; i++)
    {
        LOG(INFO) << result[i];
    }
    return 0;
}
