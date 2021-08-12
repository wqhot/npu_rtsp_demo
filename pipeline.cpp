#include <pipeline.h>
#include <time.h>
#include <fcntl.h>

Pipeline::Pipeline(std::string camera_address, std::shared_ptr<GLHelper> &gl_helper)
    : interface("../model/model.cambricon", std::bind(&Pipeline::got_detect_frame, this, std::placeholders::_1))
{
    rows = 1080;
    cols = 1920;
    rgbaQueue = jmgpuConcurrentQueueCreate();
    context = jmgpuMediaContextCreate();
    decoder = decodeCreate(context, cols, rows, 66);
    decodeStart(decoder);
    url = camera_address;
    glhelper_ = gl_helper;
    global_run_flag = true;

    std::vector<gl_item_info> item_info;
    item_info.push_back({1920, 1080, -1, -1, 1, 1, TEX_GPU});
    item_info.push_back({1920, 1080, -1, -1, 1, 1, TEX_MAT});

    glhelper_->register_item(item_info.size(), item_info);

    return;
}

void Pipeline::got_frame()
{
    cond_.notify_all();
}

Pipeline::~Pipeline()
{
}

void Pipeline::start()
{
    std::thread client(rstp_client_thread, this);
    client.detach();

    std::thread roll(decode, this);
    roll.detach();

    std::thread refresh(get_frame, this);
    refresh.detach();
}

void Pipeline::rstp_client_thread(Pipeline *pipe)
{
    startRtspClientNew(pipe->decoder, pipe->url.c_str());
}

void Pipeline::decode(Pipeline *pipe)
{
    JmgpuVideoBufferInfo bufferInfo;
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
            // pop
            int queue_size = jmgpuConcurrentQueueSize(pipe->rgbaQueue);
            while (queue_size > 3 && pipe->global_run_flag)
            {
                printf("pop: %d\n", queue_size);
                JmgpuVideoBuffer videoBuffer_tmp = jmgpuConcurrentQueueFirst(pipe->rgbaQueue);
                jmgpuMediaCodecReleaseOutputBuffer(pipe->decoder->decode, videoBuffer_tmp);
                queue_size = jmgpuConcurrentQueueSize(pipe->rgbaQueue);
            }
            jmgpuConcurrentQueueAddData(pipe->rgbaQueue, videoBuffer, NULL);
            pipe->got_frame();
        }
    }
}

void Pipeline::got_detect_frame(cv::Mat &image)
{
    glhelper_->update_tex(1, image);
}

bool Pipeline::get_frame(Pipeline *pipe)
{
    unsigned long id = 0;
    while (pipe->global_run_flag)
    {
        std::unique_lock<std::mutex> lock(pipe->mtx_);
        pipe->cond_.wait(lock);
        printf("refresh\n");
        JmgpuVideoBuffer videoBuffer = jmgpuConcurrentQueueFirst(pipe->rgbaQueue);
        if (videoBuffer)
        {
            JmgpuVideoBufferInfo bufferInfo;
            jmgpuVideoBufferGetBufferInfo(videoBuffer, &bufferInfo);
            pipe->glhelper_->update_tex(0, &videoBuffer);
            pipe->glhelper_->refresh();
            cv::Mat gpu_mat(cv::Size(pipe->cols, pipe->rows), CV_MAKE_TYPE(8, 4), cv::Scalar(0));
            jmgpuVideoBufferReadFb(videoBuffer, gpu_mat.data, pipe->cols * pipe->rows * 4);
            pipe->interface.push_image(gpu_mat, id++);
            pipe->glhelper_->wait_draw();
            jmgpuMediaCodecReleaseOutputBuffer(pipe->decoder->decode, videoBuffer);
        }
    }

    return true;
}