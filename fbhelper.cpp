#include <fbhelper.h>
#include <glog/logging.h>
#include <thread>
#include <sstream>
#include <string>

#include <errno.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#define fb_width(fb) ((fb)->w)
#define fb_height(fb) ((fb)->h)
#define fb_size(fb) ((fb)->h * (fb)->line_length)

FramebufferHelper::FramebufferHelper()
{
    linux_fb_open("/dev/fb0");

    std::thread th(loop, this);
    th.detach();
}

FramebufferHelper::~FramebufferHelper()
{
    linux_fb_close();
}

void FramebufferHelper::cp2fb(cv::Mat &image)
{
    if (fb_->bpp == 2)
    {
        rgb8888_to_rbg565(image);
    }
    unsigned int max_cols = image.cols < fb_->w ? image.cols : fb_->w;
    unsigned int max_rows = image.rows < fb_->h ? image.rows : fb_->h;
    // TicToc tic;
    for (unsigned int i = 0; i < max_rows; ++i)
    {
        size_t start_pos_screen = (fb_->line_length * i);
        size_t start_pos_image = (image.cols * i) * fb_->bpp;
        memcpy(fb_->data + start_pos_screen, image.data + start_pos_image, max_cols * fb_->bpp);
        // printf("write done.\n");
        // memcpy(image.data + start_pos_image, fb_->data + start_pos_screen, max_cols * fb_->bpp);
        // printf("read done.\n");
    }
    
}

void FramebufferHelper::linux_fb_close()
{
    if(fb_ == NULL)
    {
        return;
    }
    munmap(fb_->data, fb_size(fb_));
    close(fb_->fd);
    memset(fb_, 0x00, sizeof(fb_info_t));
    free(fb_);
    fb_ == NULL;
}

void FramebufferHelper::rgb8888_to_rbg565(cv::Mat &image)
{
    if (image.channels() == 4)
    {
        cv::cvtColor(image, image, cv::COLOR_BGRA2BGR565);
    }
    else if (image.channels() == 3)
    {
        cv::cvtColor(image, image, cv::COLOR_BGR2BGR565);
    }
    else if (image.channels() == 1)
    {
        cv::cvtColor(image, image, cv::COLOR_GRAY2BGR565);
    }

    return;
}

void FramebufferHelper::register_item(int item_num, std::vector<gl_item_info> &tex_info)
{
    tex_name_.resize(item_num);
    tex_data_.resize(item_num);
    tex_ptr_.resize(item_num);
    tex_gpu_.resize(item_num, nullptr);

    need_refresh_.resize(item_num, true);

    for (int i = 0; i != item_num; ++i)
    {
        cv::Mat img(cv::Size(tex_info[i].w, tex_info[i].h), CV_MAKE_TYPE(8, 4), cv::Scalar(0, 0, 0, 0));
        tex_info_.push_back(tex_info[i]);
        tex_info_[i].ld_x = (tex_info_[i].ld_x + 1.0) / 2.0;
        tex_info_[i].ld_y = (tex_info_[i].ld_y + 1.0) / 2.0;
        tex_info_[i].rt_x = (tex_info_[i].rt_x + 1.0) / 2.0;
        tex_info_[i].rt_y = (tex_info_[i].rt_y + 1.0) / 2.0;
        texs.push_back(img);
    }
}

void FramebufferHelper::linux_fb_open(const char *filename)
{
    uint32_t size = 0;
    fb_ = NULL;
    struct fb_fix_screeninfo fix;
    struct fb_var_screeninfo var;

    fb_ = (fb_info_t *)calloc(1, sizeof(fb_info_t));

    fb_->fd = open(filename, O_RDWR);
    if (fb_->fd < 0)
    {
        free(fb_);
        return;
    }

    if (ioctl(fb_->fd, FBIOGET_FSCREENINFO, &fix) < 0)
        goto fail;
    if (ioctl(fb_->fd, FBIOGET_VSCREENINFO, &var) < 0)
        goto fail;

    var.xoffset = 0;
    var.yoffset = 0;
    ioctl(fb_->fd, FBIOPAN_DISPLAY, &(var));

    LOG(INFO) << "fb_info_t: " << filename;
    LOG(INFO) << "fb_info_t: xres=" << var.xres << " yres=" << var.yres << " bits_per_pixel=" << var.bits_per_pixel;
    LOG(INFO) << "fb_info_t: red(" << var.red.offset << " " << var.red.length
              << ") green(" << var.green.offset << " " << var.green.length
              << ") blue(" << var.blue.offset << " " << var.blue.length;

    fb_->w = var.xres;
    fb_->h = var.yres;
    fb_->bpp = var.bits_per_pixel / 8;
    fb_->line_length = fix.line_length;

    size = fb_size(fb_);
    fb_->data = (uint8_t *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_->fd, 0);

    if (fb_->data == MAP_FAILED)
    {
        printf("map framebuffer failed.\n");
        goto fail;
    }

    LOG(INFO) << "line_length=" << fix.line_length;
    LOG(INFO) << "xres_virtual=" << var.xres_virtual << " yres_virtual=" << var.yres_virtual
              << " xpanstep=" << fix.xpanstep << " ywrapstep=" << fix.ywrapstep;
    // std::stringstream fmt;
    // std::stringstream ffmt;
    // fmt << "mmap address=" << std::hex << fb_->data;
    // LOG(INFO) << fmt.str();

    return;
fail:
    printf("%s is not a framebuffer.\n", filename);
    close(fb_->fd);
    free(fb_);

    return;
}

void FramebufferHelper::wait_draw()
{
    std::unique_lock<std::mutex> lock(mtx_);
    lock.unlock();
}

void FramebufferHelper::loop(FramebufferHelper *helper)
{
    while(helper->fb_ != NULL)
    {
        helper->wait_refresh();
    }
}

void FramebufferHelper::wait_refresh()
{
    std::unique_lock<std::mutex> lock(refresh_mtx_);
    refresh_cond_.wait(lock);
    paint_gl();
}

void FramebufferHelper::refresh()
{
    refresh_cond_.notify_one();
}

void FramebufferHelper::update_gpu_tex(int tex_index, JmgpuVideoBuffer *ptr)
{
    std::unique_lock<std::mutex> lock(mtx_);
    // 复制到内存
    jmgpuVideoBufferReadFb(ptr, texs[tex_index].data, tex_info_[tex_index].w * tex_info_[tex_index].h * 4);
    lock.unlock();
    need_refresh_[tex_index] = 1;
}

void FramebufferHelper::update_tex(int tex_index, void *ptr)
{
    std::unique_lock<std::mutex> lock(mtx_);
    memcpy(texs[tex_index].data, ptr, tex_info_[tex_index].w * tex_info_[tex_index].h * 4);
    need_refresh_[tex_index] = 1;
    lock.unlock();
}

void FramebufferHelper::update_tex(int tex_index, cv::Mat &data)
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
        texs[tex_index] = std::move(temp);
    }
    else if (data.channels() == 1)
    {
        cv::Mat temp;
        cv::cvtColor(data, temp, cv::COLOR_GRAY2RGBA);
        texs[tex_index] = std::move(temp);
    }
    else
    {
        texs[tex_index] = std::move(data);
    }
    need_refresh_[tex_index] = 1;
    lock.unlock();
}

void FramebufferHelper::resize_gl(int w, int h)
{
    
}

void FramebufferHelper::paint_gl()
{
    static float fps = 0;
    static double now_stamp = get_current_time();
    static double last_stamp = get_current_time();

    TicToc tic_all;
    
    std::unique_lock<std::mutex> lock(mtx_);
    cv::Mat img(cv::Size(1920, 1080), CV_MAKE_TYPE(8, 4), cv::Scalar(0, 0, 0, 0));
    for (int i = 0; i != tex_name_.size(); ++i)
    {
        if (texs[i].rows != tex_info_[i].h || texs[i].cols != tex_info_[i].w)
        {
            continue;
        }
        // std::cout << texs[i].size() << std::endl;
        // 无需刷新时
        if (need_refresh_[i] == true)
        {
            cv::Rect rect(
                cv::Point2f(tex_info_[i].ld_x * 1920, tex_info_[i].ld_y * 1080),
                cv::Point2f(tex_info_[i].rt_x * 1920, tex_info_[i].rt_y * 1080)
            );
            cv::Mat roi = img(rect);
            cv::Mat dst;
            cv::resize(texs[i], dst, roi.size(), 0.0, 0.0, cv::INTER_NEAREST);
            cv::addWeighted(roi, 1.0, dst, 1.0, 0.0, roi);
            
        }
        need_refresh_[i] = 0;

        // printf("painting %d...\n", i);
    }
    // printf("paint over\n");
    lock.unlock();
    cp2fb(img);
    now_stamp = get_current_time();
    fps = 0.8 * fps + 0.2 * 1.0 / (now_stamp - last_stamp);
    last_stamp = now_stamp;
    printf(" fps = %f\n", fps);
    LOG(INFO) << "fps = " << fps;
}