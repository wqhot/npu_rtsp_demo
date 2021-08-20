#include <pipeline.h>
#include <time.h>
#include <fcntl.h>
#include <gflags/gflags.h>

DECLARE_bool(use_npu);
DECLARE_bool(use_rtsp);
DECLARE_bool(use_rtsp_for_npu);
DECLARE_bool(display_rtsp);
DECLARE_bool(display_detect);
DECLARE_uint32(blend_more);

static std::vector<unsigned char> img_data;
#define checkImageWidth1 1920
#define checkImageHeight1 64
#define checkImageWidth2 64
#define checkImageHeight2 1080

static GLubyte otherImage[checkImageHeight1][checkImageWidth1][4];
static GLubyte otherImage2[checkImageHeight2][checkImageWidth2][4];

void makeCheckImages(void)
{
    int i, j, c;

    for (i = 0; i < checkImageHeight1; i++)
    {
        for (j = 0; j < checkImageWidth1; j++)
        {
            c = ((((i & 0x10) == 0) ^ ((j & 0x10)) == 0)) * 255;
            otherImage[i][j][0] = (GLubyte)0;
            otherImage[i][j][1] = (GLubyte)0;
            otherImage[i][j][2] = (GLubyte)c;
            otherImage[i][j][3] = (GLubyte)c;
        }
    }
    for (i = 0; i < checkImageHeight2; i++)
    {
        for (j = 0; j < checkImageWidth2; j++)
        {
            c = ((((i & 0x10) == 0) ^ ((j & 0x10)) == 0)) * 255;
            otherImage2[i][j][0] = (GLubyte)c;
            otherImage2[i][j][1] = (GLubyte)0;
            otherImage2[i][j][2] = (GLubyte)0;
            otherImage2[i][j][3] = (GLubyte)c;
        }
    }
}

Pipeline::Pipeline(std::string camera_address, std::shared_ptr<GLHelper> &gl_helper)
    //加载目标检测模型文件，绑定回调函数
    : interface("../model/model.cambricon", std::bind(&Pipeline::got_detect_frame, this, std::placeholders::_1))
{
    // 解码相关
    rows = 1080;
    cols = 1920;
    if (FLAGS_use_rtsp)
    {
        rgbaQueue = jmgpuConcurrentQueueCreate();
        context = jmgpuMediaContextCreate();
        decoder = decodeCreate(context);
        decodeStart(decoder);
    }
    url = camera_address;
    glhelper_ = gl_helper;
    global_run_flag = true;

    // 注册opengl显示图层
    std::vector<gl_item_info> item_info;
    // 图层0为解码图像显示图层，纹理类型为显存图像
    item_info.push_back({1920, 1080, -1, -1, 1, 1, TEX_GPU});
    // 图层1为目标检测结果图层，纹理类型为opencv图像
    item_info.push_back({1920, 1080, -1, -1, 1, 1, TEX_MAT});

    makeCheckImages();
    for (uint32_t i = 0; i < FLAGS_blend_more; i++)
    {
        if (i % 2)
        {
            item_info.push_back({checkImageWidth1, checkImageHeight1, -1, -1 + 0.0625f * (i - 1) / 2 , 1, -0.9375f + 0.0625f * (i - 1) / 2, TEX_MAT});
        }
        else
        {
            item_info.push_back({checkImageWidth2, checkImageHeight2, -1 + 0.06f * i / 2, -1, -0.94f + 0.06f * i / 2, 1, TEX_MAT});
        }
    }

    glhelper_->register_item(item_info.size(), item_info);

    return;
}

void Pipeline::got_frame()
{
    // 为了防止阻塞显卡硬解接收队列，硬解回调函数仅仅发出通知
    cond_.notify_all();
}

Pipeline::~Pipeline()
{
}

void Pipeline::start()
{
    // rtsp流线程
    std::thread client(rstp_client_thread, this);
    client.detach();
    // 发送解码结果线程
    std::thread roll(decode, this);
    roll.detach();
    // 接收解码结果线程
    std::thread refresh(get_frame, this);
    refresh.detach();
}

void Pipeline::rstp_client_thread(Pipeline *pipe)
{
    if (FLAGS_use_rtsp)
    {
        startRtspClientNew(pipe->decoder, pipe->url.c_str());
    }
}

void Pipeline::decode(Pipeline *pipe)
{
    JmgpuVideoBufferInfo bufferInfo;
    int firstNotDisplayNum = 3;
    // 无需显卡解码时
    if (!FLAGS_use_rtsp)
    {
        while (pipe->global_run_flag)
        {
            pipe->got_frame();
            std::this_thread::sleep_for(std::chrono::milliseconds(33));
        }
        return;
    }
    if (pipe->decoder == NULL)
    {
        return;
    }
    while (pipe->global_run_flag)
    {
        int64_t curTimeStamp = jmgpuCurrentTimeStamp();
        JmgpuVideoBuffer videoBuffer = jmgpuMediaCodecDequeueOutputBuffer(pipe->decoder->decode, -1);
        if (videoBuffer)
        {
            jmgpuVideoBufferGetBufferInfo(videoBuffer, &bufferInfo);
            int64_t displayTime = bufferInfo.pts;
            int64_t curTime = jmgpuCurrentTimeStamp();
            int64_t delay = curTime - displayTime;
            if ((firstNotDisplayNum > 0) || (delay > 20))
            {
                jmgpuMediaCodecReleaseOutputBuffer(pipe->decoder->decode, videoBuffer);
                firstNotDisplayNum--;
            }
            else
            {
                jmgpuConcurrentQueueAddData(pipe->rgbaQueue, videoBuffer, NULL);
                // 通知接收解码结果线程
                pipe->got_frame();
            }
        }
        // 如果rgbaQueue过长，需要pop
        int queue_size = jmgpuConcurrentQueueSize(pipe->rgbaQueue);
        while (queue_size > 3 && pipe->global_run_flag)
        {
            // printf("pop: %d\n", queue_size);
            LOG(INFO) << "pop" << queue_size;
            JmgpuVideoBuffer videoBuffer_tmp = jmgpuConcurrentQueueFirst(pipe->rgbaQueue);
            jmgpuMediaCodecReleaseOutputBuffer(pipe->decoder->decode, videoBuffer_tmp);
            queue_size = jmgpuConcurrentQueueSize(pipe->rgbaQueue);
        }
    }
}

// 目标检测回调
void Pipeline::got_detect_frame(cv::Mat &image)
{
    if (FLAGS_display_detect)
    {
        glhelper_->update_tex(1, image);
    }
    for (uint32_t i = 0; i < FLAGS_blend_more; i++)
    {
        if (i % 2)
        {
            cv::Mat img(cv::Size(checkImageWidth1, checkImageHeight1), CV_MAKETYPE(8, 4), otherImage);
            glhelper_->update_tex(2 + i, img);
        }
        else
        {
            cv::Mat img(cv::Size(checkImageWidth2, checkImageHeight2), CV_MAKETYPE(8, 4), otherImage2);
            glhelper_->update_tex(2 + i, img);
        }
    }
}

bool Pipeline::get_frame(Pipeline *pipe)
{
    unsigned long id = 0;
    cv::Mat test_image = cv::imread("../video/0750.bmp");
    cv::resize(test_image, test_image, cv::Size(1920, 1080));
    while (pipe->global_run_flag)
    {
        std::unique_lock<std::mutex> lock(pipe->mtx_);
        pipe->cond_.wait(lock);
        printf("refresh\n");
        if (FLAGS_use_rtsp)
        {
            JmgpuVideoBuffer videoBuffer = jmgpuConcurrentQueueFirst(pipe->rgbaQueue);
            if (videoBuffer)
            {
                JmgpuVideoBufferInfo bufferInfo;
                jmgpuVideoBufferGetBufferInfo(videoBuffer, &bufferInfo);
                // 更新opengl纹理，开始异步刷新
                if (FLAGS_display_rtsp)
                {
                    pipe->glhelper_->update_tex(0, &videoBuffer);
                }
                pipe->glhelper_->refresh();
                cv::Mat gpu_mat(cv::Size(pipe->cols, pipe->rows), CV_MAKE_TYPE(8, 4), cv::Scalar(0));
                // 复制到内存，并发送到目标检测接收区
                if (FLAGS_use_rtsp_for_npu)
                {
                    jmgpuVideoBufferReadFb(videoBuffer, gpu_mat.data, pipe->cols * pipe->rows * 4);
                }
                else
                {
                    gpu_mat = test_image;
                }
                if (FLAGS_use_npu)
                {
                    pipe->interface.push_image(gpu_mat, id++);
                }
                // 等待前面的刷新结束后释放显存
                pipe->glhelper_->wait_draw();
                jmgpuMediaCodecReleaseOutputBuffer(pipe->decoder->decode, videoBuffer);
            }
        }
        else
        {
            cv::Mat gpu_mat = test_image;
            pipe->glhelper_->refresh();
            if (FLAGS_use_npu)
            {
                pipe->interface.push_image(gpu_mat, id++);
            }
        }
    }

    return true;
}