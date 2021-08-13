#include "glhelper.h"
#include <glog/logging.h>

GLHelper::GLHelper()
{
    init_gl();
}

void GLHelper::init_gl()
{
    // glutInitContextVersion(1, 5); //初始化OpenGL所需文件，设置版本为2.1
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("xxxx");

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glShadeModel(GL_FLAT);
    glDisable(GL_DEPTH_TEST);

    //enble blend and set blend funtion, 开启融合，并设置融合方式
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendFunc(GL_ONE, GL_ONE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void GLHelper::register_item(int item_num, std::vector<gl_item_info> &tex_info)
{
    tex_name_.resize(item_num);
    tex_data_.resize(item_num);
    tex_ptr_.resize(item_num);
    tex_gpu_.resize(item_num, nullptr);
    need_refresh_.resize(item_num, true);
    glGenTextures(item_num, tex_name_.data());

    for (int i = 0; i != item_num; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, tex_name_[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        tex_info_.push_back(tex_info[i]);
        tex_ptr_[i] = (GLubyte *)malloc(tex_info_[i].w * tex_info_[i].h * 4);
        if (i > 0)
        {
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        }
    }

    glEnable(GL_TEXTURE_2D);
}

void GLHelper::wait_draw()
{
    std::unique_lock<std::mutex> lock(mtx_);
    lock.unlock();
}

void GLHelper::wait_refresh()
{
    std::unique_lock<std::mutex> lock(refresh_mtx_);
    refresh_cond_.wait(lock);
    paint_gl();
}

void GLHelper::refresh()
{
    refresh_cond_.notify_one();
}

void GLHelper::update_tex(int tex_index, JmgpuVideoBuffer* ptr)
{
    std::unique_lock<std::mutex> lock(mtx_);
    tex_gpu_[tex_index] = ptr;
    lock.unlock();
}

void GLHelper::update_tex(int tex_index, void *ptr)
{
    std::unique_lock<std::mutex> lock(mtx_);
    memcpy(tex_ptr_[tex_index], ptr, tex_info_[tex_index].w * tex_info_[tex_index].h * 4);
    need_refresh_[tex_index] = 1;
    lock.unlock();
}

void GLHelper::update_tex(int tex_index, cv::Mat &data)
{
    std::unique_lock<std::mutex> lock(mtx_);
    if (data.rows * data.cols != tex_info_[tex_index].w * tex_info_[tex_index].h)
    {
        return;
    }
    if (data.channels() == 3)
    {
        cv::Mat temp;
        cv::cvtColor(data, temp, cv::COLOR_RGB2RGBA);
        tex_data_[tex_index] = std::move(temp);
    }
    else
    {
        tex_data_[tex_index] = std::move(data);
    }
    need_refresh_[tex_index] = 1;
    lock.unlock();
}

void GLHelper::resize_gl(int w, int h)
{
    //set videwport 设置视口
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    //set projection matrix, 设置投影矩阵
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    //set model videw matrix, 设置模型视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLHelper::paint_gl()
{
    static float fps = 0;
    static double now_stamp = get_current_time();
    static double last_stamp = get_current_time();

    TicToc tic_all;
    
    std::unique_lock<std::mutex> lock(mtx_);
    //清除缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i != tex_name_.size(); ++i)
    {
        glBindTexture(GL_TEXTURE_2D, tex_name_[i]);

        if (tex_info_[i].type == TEX_MAT)
        {
            if (tex_data_[i].rows != tex_info_[i].h || tex_data_[i].cols != tex_info_[i].w)
            {
                continue;
            }
            // 无需刷新时
            if (need_refresh_[i] == true)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_info_[i].w,
                             tex_info_[i].h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                             tex_data_[i].data);
            }
            if (i != 0)
            {
                need_refresh_[i] = 0;
            }
        }
        else if (tex_info_[i].type == TEX_GPU)
        {
            need_refresh_[i] = 1;
            if ((tex_gpu_[i]) != nullptr)
            {
                void *rgbaAddr = jmgpuVideoBufferGetFbAddr(*(tex_gpu_[i]));
                glTexImage2D(GL_TEXTURE_2D, 0, 0,
                             tex_info_[i].w, tex_info_[i].h,
                             0, GL_BGRA, GL_UNSIGNED_BYTE,
                             (GLvoid *)rgbaAddr);
                tex_gpu_[i] = nullptr;
            }
        }
        else if (tex_info_[i].type == TEX_PTR)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_info_[i].w,
                         tex_info_[i].h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         (GLvoid *)tex_ptr_[i]);
        }

        glBegin(GL_QUADS);
        // 左下角
        glTexCoord2f(0.0, 1.0);
        glVertex3f(tex_info_[i].ld_x, tex_info_[i].ld_y, 0.0);
        // 右下角
        glTexCoord2f(1.0, 1.0);
        glVertex3f(tex_info_[i].rt_x, tex_info_[i].ld_y, 0.0);
        // 右上角
        glTexCoord2f(1.0, 0.0);
        glVertex3f(tex_info_[i].rt_x, tex_info_[i].rt_y, 0.0);
        // 左上角
        glTexCoord2f(0.0, 0.0);
        glVertex3f(tex_info_[i].ld_x, tex_info_[i].rt_y, 0.0);
        glEnd();
        // printf("painting %d...\n", i);
    }
    lock.unlock();
    tic_all.toc_print("set texture");
    glutSwapBuffers();
    tic_all.toc_print("swapbuffers");
    now_stamp = get_current_time();
    fps = 0.8 * fps + 0.2 * 1.0 / (now_stamp - last_stamp);
    last_stamp = now_stamp;
    printf(" fps = %f\n", fps);
    LOG(INFO) << "fps = " << fps;
}
