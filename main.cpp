#include "glhelper.h"
#include <glog/logging.h>
#include <pipeline.h>
#include <gflags/gflags.h>

DEFINE_bool(use_npu, true, "是否使用目标检测");
DEFINE_bool(use_rtsp, true, "是否使用RTSP解码");
DEFINE_bool(use_rtsp_for_npu, true, "是否将RTSP解码图像用于目标检测");
DEFINE_bool(display_rtsp, true, "是否显示rtsp解码图像");
DEFINE_bool(display_detect, true, "是否显示目标检测结果");

std::shared_ptr<GLHelper> glhelper_;

void loop(int)
{
    glhelper_->wait_refresh();
    glutTimerFunc(0, loop, 0);
};

static void reshape(int w, int h)
{
    glhelper_->resize_gl(w, h);
}

static void display()
{
    glhelper_->wait_refresh();
}

int main(int argc, char **argv)
{
    setenv("MWV206_GLVERSION", "1", 1);
    setenv("MWV206_HW_DEC", "1", 1);
    
    FLAGS_log_dir = "./";
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    LOG(INFO) << "是否使用目标检测: " << FLAGS_use_npu;
    LOG(INFO) << "是否使用RTSP解码: " << FLAGS_use_rtsp;
    LOG(INFO) << "是否将RTSP解码图像用于目标检测: " << FLAGS_use_rtsp_for_npu;
    LOG(INFO) << "是否显示rtsp解码图像: " << FLAGS_display_rtsp;
    LOG(INFO) << "是否显示目标检测结果: " << FLAGS_display_detect;


    // 初始化opengl显示界面
    glutInit(&argc, argv);
    glhelper_ = std::make_shared<GLHelper>();

    // 用于管理接收到的图像并发送到目标检测接口中与opengl显示
    Pipeline pipe(argv[1], glhelper_);
    pipe.start();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(0, loop, 0);
    glutMainLoop();
    return 0;
}