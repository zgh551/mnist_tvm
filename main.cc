/*
 * mnist module
 * */
// tvm 
#include <dlpack/dlpack.h>
#include <tvm/runtime/module.h>
#include <tvm/runtime/packed_func.h>
#include <tvm/runtime/registry.h>
// opencv 
#include <opencv4/opencv2/opencv.hpp>
// system
#include <cstdio>
#include <fstream>


int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        LOG(INFO) << "[mnist tvm]:Image Path: " << argv[1];
        LOG(INFO) << "[mnist tvm]:Dynamic Lib Path: " << argv[2];
        LOG(INFO) << "[mnist tvm]:Parameter Path: " << argv[3];
    }
    else
    {
        LOG(INFO) << "executor [img] [mod lib] [mod param]";
        return -1;
    }
    LOG(INFO) << "[mnist tvm]:Soft Version: V" << MNIST_VERSION;

    // read the image
    cv::Mat image, gray_image;
    image = cv::imread(argv[1]);
    if(image.data == nullptr){
        LOG(INFO) << "[mnist tvm]:Image don't exist!";
        return 0;
    }
    else{
        cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
        gray_image.convertTo(gray_image, CV_32FC3);

        LOG(INFO) << "[mnist tvm]:---Load Image--";
        LOG(INFO) << "[mnist tvm]:Image size: " << gray_image.rows << " X " << gray_image.cols;
        // cv::imshow("mnist image", gray_image);
        // cv::waitKey(0);
    }

    // create tensor
    DLTensor *x;
    DLTensor *y;
    int input_ndim  = 4;
    int output_ndim = 2;
    int64_t input_shape[4]  = {1, 1, gray_image.rows, gray_image.cols};
    int64_t output_shape[2] = {1, 10};

    int dtype_code  = kDLFloat;
    int dtype_bits  = 32;
    int dtype_lanes = 1;
    int device_type = kDLOpenCL;
    int device_id   = 0;

    TVMByteArray params_arr;
    DLDevice dev{kDLOpenCL, 0};

    // allocate the array space
    TVMArrayAlloc(input_shape, input_ndim, dtype_code, dtype_bits, dtype_lanes, device_type, device_id, &x);
    TVMArrayAlloc(output_shape, output_ndim, dtype_code, dtype_bits, dtype_lanes, device_type, device_id, &y);

    // the memory space allocate
    std::vector<float> x_input(gray_image.rows * gray_image.cols);
    std::vector<float> y_output(10);
        
    // load the mnist dynamic lib
    LOG(INFO) << "[mnist tvm]:---Load Dynamic Lib---";
    tvm::runtime::Module mod_dylib = tvm::runtime::Module::LoadFromFile(argv[2]);
    // get the mnist module
    tvm::runtime::Module mod = mod_dylib.GetFunction("mnist")(dev);

    // load the mnist module parameters
    LOG(INFO) << "[mnist tvm]:---Load Parameters---";
    std::ifstream params_in(argv[3], std::ios::binary);
    std::string params_data((std::istreambuf_iterator<char>(params_in)), std::istreambuf_iterator<char>());
    params_in.close();
    params_arr.data = params_data.c_str();
    params_arr.size = params_data.length();
    // get load parameters function
    tvm::runtime::PackedFunc load_params = mod.GetFunction("load_params");
    load_params(params_arr);

    LOG(INFO) << "[mnist tvm]:---Set Input---";
    // get set input data function
    tvm::runtime::PackedFunc set_input = mod.GetFunction("set_input");
    // copy image data to cpu memory space
    memcpy(x_input.data(), gray_image.data, gray_image.rows * gray_image.cols * sizeof(float));
    // from cpu memory space copy data to gpu memory space
    TVMArrayCopyFromBytes(x, x_input.data(), gray_image.rows * gray_image.cols * sizeof(float));
    set_input("Input3", x);

    LOG(INFO) << "[mnist tvm]:---Run---";
    // get run function
    tvm::runtime::PackedFunc run = mod.GetFunction("run");
    run();

    LOG(INFO) << "[mnist tvm]:---Get Output---";
    // get output data function
    tvm::runtime::PackedFunc get_output = mod.GetFunction("get_output");
    get_output(0, y);
    TVMArrayCopyToBytes(y, y_output.data(), 10 * sizeof(float));

    //auto result = static_cast<float *>(y->data);
    for (int i = 0; i < 10; i++)
    {
        LOG(INFO) << y_output[i];
    }

    return 0;
}
