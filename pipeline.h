#pragma once

#include <mwv206api.h>


#include <jmgpu_media_context.h>
#include <jmgpu_media_codec.h>

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
#include <decode/concurrent_queue.h>

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
    static void draw_blend_layer_thread(Pipeline *pipe);
    void got_detect_frame(cv::Mat &image);
    void got_frame();
    void draw_blend_layer();

    std::condition_variable cond_, cond_draw_blend_layer;
    std::mutex mtx_, mtx_draw_blend_layer;
    std::string url;
    // JSSDetectInterface interface;

    std::unique_ptr<JSSDetectInterface> interface_ptr;

    JmgpuMediaContext context;
    Decode *decoder;
    ConcurrentQueue<JmgpuVideoBuffer> rgbaQueue;
    bool global_run_flag;
};

