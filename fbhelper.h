#include <displayhelper.h>

typedef struct _fb_info_t
{
    int fd;
    uint8_t *data;

    uint32_t w;
    uint32_t h;
    uint32_t bpp;
    uint32_t line_length;
} fb_info_t;

class FramebufferHelper : public DisplayHelper
{
public:
    FramebufferHelper();

    void register_item(int item_num, std::vector<gl_item_info> &tex_info);

    void update_tex(int tex_index, void *ptr);

    void update_gpu_tex(int tex_index, JmgpuVideoBuffer *ptr);

    void update_tex(int tex_index, cv::Mat &data);

    void wait_draw();

    void resize_gl(int w, int h);

    void refresh();

    void wait_refresh();

    void paint_gl();

    ~FramebufferHelper();

private:
    fb_info_t* fb_;

    void rgb8888_to_rbg565(cv::Mat &image);

    void linux_fb_open(const char *filename);
    void linux_fb_close();

    static void loop(FramebufferHelper *helper);

    void cp2fb(cv::Mat &image);

    std::vector<cv::Mat> texs;

};