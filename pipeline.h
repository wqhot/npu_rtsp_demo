#pragma once

#include <mwv206api.h>

#include <jmgpu_concurrent_queue.h>
#include <jmgpu_media_context.h>
#include <jmgpu_media_codec.h>
#include <jmgpu_time.h>
#include <decode.h>
#include <rtsp_client.h>
#include <glhelper.h>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <opencv4/opencv2/opencv.hpp>
#include <detect_interface.h>

class Pipeline
{
public:
    Pipeline(std::string camera_address, std::shared_ptr<GLHelper> &gl_helper);
    ~Pipeline();
    void start();

private:
    int rows;
    int cols;
    std::shared_ptr<GLHelper> glhelper_;
    static bool get_frame(Pipeline *pipe);
    static void decode(Pipeline *pipe);
    static void rstp_client_thread(Pipeline *pipe);
    void got_detect_frame(cv::Mat &image);
    void got_frame();

    std::condition_variable cond_;
    std::mutex mtx_;
    std::string url;
    JSSDetectInterface interface;

    JmgpuMediaContext context;
    Decode *decoder;
    JmgpuConcurrentQueue *rgbaQueue;
    bool global_run_flag;
};
