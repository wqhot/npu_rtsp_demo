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
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <condition_variable>
#include <sys/time.h>
#include <unistd.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <jmgpu_video_buffer.h>
#include <opencv4/opencv2/opencv.hpp>

typedef enum
{
TEX_MAT = 0,
TEX_GPU,
TEX_PTR
} tex_type;

typedef struct
{
    int w;
    int h;
    float ld_x;
    float ld_y;
    float rt_x;
    float rt_y;
    tex_type type = TEX_MAT;
} gl_item_info;

class GLHelper
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
    std::vector<GLuint> tex_name_;
    std::vector<cv::Mat> tex_data_;
    std::vector<gl_item_info> tex_info_;
    std::vector<GLubyte*> tex_ptr_;
    std::vector<JmgpuVideoBuffer*> tex_gpu_;
    std::vector<int> need_refresh_;

    std::condition_variable cond_;
    std::mutex mtx_;

    std::mutex refresh_mtx_;
    std::condition_variable refresh_cond_;
};


class TicToc
{
public:
    TicToc()
    {
        tic();
    }

    void tic()
    {
        start = std::chrono::system_clock::now();
    }

    double toc()
    {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        return elapsed_seconds.count() * 1000;
    }

    double restart_toc()
    {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        start = std::chrono::system_clock::now();
        return elapsed_seconds.count() * 1000;
    }

    void record_time()
    {
        double time_interval_ = restart_toc();
        // LOG(INFO) << "Interval time: " << time_interval_ << "ms";
    }

    void toc_print(std::string str)
    {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << str << " spend: " << elapsed_seconds.count() * 1000 << " ms" << std::endl;
        start = std::chrono::system_clock::now();
    }

    void log(std::string str)
    {
        double time_interval_ = restart_toc();
        // LOG(INFO) << str << ": " << time_interval_ << " ms";
    }

private:
    std::chrono::time_point<std::chrono::system_clock> start, end;
};

inline double get_current_time()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz); 
    return double(tv.tv_sec) + double(tv.tv_usec) / 1000000.0;
}