#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <string.h>
#include <iostream>
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>

#include <condition_variable>
#include <jmgpu_video_buffer.h>
#include <displayhelper.h>
#include <opencv4/opencv2/opencv.hpp>


class GLHelper : public DisplayHelper
{
public:
    GLHelper();

    void init_gl();

    void register_item(int item_num, std::vector<gl_item_info> &tex_info);

    void update_tex(int tex_index, void* ptr);

    void update_gpu_tex(int tex_index, JmgpuVideoBuffer* ptr);

    void update_tex(int tex_index, cv::Mat &data);

    void wait_draw();

    void resize_gl(int w, int h);

    void refresh();

    void wait_refresh();

    void paint_gl();

private:
};