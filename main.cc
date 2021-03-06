// tvm 
#include <dlpack/dlpack.h>
#include <tvm/runtime/module.h>
#include <tvm/runtime/packed_func.h>
#include <tvm/runtime/registry.h>
#include <tvm/runtime/c_runtime_api.h>
// opencv 
#include <opencv4/opencv2/opencv.hpp>
//#include <opencv2/opencv.hpp>
// system
#include <cstdio>
#include <fstream>
#include <sys/time.h>

double GetCurTime(void)
{
    struct timeval tm;
    gettimeofday(&tm, 0);
    return tm.tv_usec + tm.tv_sec * 1000000;
}



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
        LOG(INFO) << "exe [imag path] [module dynamic lib path] [module parameter path]";
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
        // LOG(INFO) << gray_image;
        // cv::imshow("mnist image", gray_image);
        // cv::waitKey(0);
    }

    std::vector<float> y_output(10);
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
    int device_id   = 0;
#ifdef CPU 
    int device_type = kDLCPU;
    LOG(INFO) << "[mnist tvm]:---Device Type Configure:CPU---";
#elif OpenCL 
    int device_type = kDLOpenCL;
    LOG(INFO) << "[mnist tvm]:---Device Type Configure:OPENCL---";
#endif
    TVMByteArray params_arr;
    DLDevice dev{static_cast<DLDeviceType>(device_type), device_id};

    // allocate the array space
    TVMArrayAlloc(input_shape, input_ndim, dtype_code, dtype_bits, dtype_lanes, device_type, device_id, &x);
    TVMArrayAlloc(output_shape, output_ndim, dtype_code, dtype_bits, dtype_lanes, device_type, device_id, &y);

    // load the mnist dynamic lib
    LOG(INFO) << "[mnist tvm]:---Load Dynamic Lib--";
    tvm::runtime::Module mod_dylib = tvm::runtime::Module::LoadFromFile(argv[2]);
    // load the mnist module parameters
    LOG(INFO) << "[mnist tvm]:---Load Parameters--";
    std::ifstream params_in(argv[3], std::ios::binary);
    std::string params_data((std::istreambuf_iterator<char>(params_in)), std::istreambuf_iterator<char>());
    params_in.close();
    params_arr.data = params_data.c_str();
    params_arr.size = params_data.length();

    // get the mnist module
    tvm::runtime::Module mod = mod_dylib.GetFunction("mnist")(dev);
    // get set input data function
    tvm::runtime::PackedFunc set_input = mod.GetFunction("set_input");
    // get load parameters function
    tvm::runtime::PackedFunc load_params = mod.GetFunction("load_params");
    // get run function
    tvm::runtime::PackedFunc run = mod.GetFunction("run");
    // get output data function
    tvm::runtime::PackedFunc get_output = mod.GetFunction("get_output");
    
    TVMArrayCopyFromBytes(x, gray_image.data, gray_image.rows * gray_image.cols * sizeof(float));

    set_input("Input3", x);
    load_params(params_arr);

    double t1 = GetCurTime();
    run();
    double t2 = GetCurTime();
    get_output(0, y);
    TVMArrayCopyToBytes(y, y_output.data(), 10 * sizeof(float));
    double t3 = GetCurTime();
    TVMArrayFree(x);
    TVMArrayFree(y);
    double t4 = GetCurTime();

    //auto result = static_cast<float *>(y->data);
    for (int i = 0; i < 10; i++)
    {
        LOG(INFO) << y_output[i];
    }
    LOG(INFO) << "[mnist tvm]:---Executor Time(run):" << t2 - t1 << "[us]";
    LOG(INFO) << "[mnist tvm]:---Executor Time(get_output):" << t3 - t2 << "[us]";
    LOG(INFO) << "[mnist tvm]:---Executor Time(Free):" << t4 - t3 << "[us]";

    return 0;
}
