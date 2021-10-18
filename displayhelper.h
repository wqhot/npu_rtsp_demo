#pragma once
#include <vector>
#include <jmgpu_video_buffer.h>
#include <condition_variable>
#include <mutex>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <sys/time.h>
#include <unistd.h>
#include <opencv4/opencv2/opencv.hpp>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>


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


class DisplayHelper
{
public:
    virtual void register_item(int item_num, std::vector<gl_item_info> &tex_info){};

    virtual void update_tex(int tex_index, void* ptr){};

    virtual void update_gpu_tex(int tex_index, JmgpuVideoBuffer* ptr){};

    virtual void update_tex(int tex_index, cv::Mat &data){};

    virtual void wait_draw(){};

    virtual void resize_gl(int w, int h){};

    virtual void refresh(){};

    virtual void wait_refresh(){};

    virtual void paint_gl(){};

protected:
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

inline double get_current_time()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz); 
    return double(tv.tv_sec) + double(tv.tv_usec) / 1000000.0;
}

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