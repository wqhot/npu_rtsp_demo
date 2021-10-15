#include <decode.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

Decode *decodeCreate(JmgpuMediaContext context)
{
	Decode *decode = calloc(1, sizeof(Decode));
	JmgpuVideoParams videoParams;
	videoParams.inputFormat = JMV_VIDEO_DATA_FORMAT_H264;
	videoParams.outputFormat = JMV_VIDEO_DATA_FORMAT_R8G8B8A8;

	videoParams.outputVideoWidth = JMV_INVALID_VIDEO_SIZE;//设置无效值，sdk内部会设置成视频原有的宽度
	videoParams.outputVideoWidth = JMV_INVALID_VIDEO_SIZE;//设置无效值，sdk内部会设置成视频原有的高度
	videoParams.codecMode = JMV_MEDIA_CODEC_ASYNC_MODE;
	// videoParams.codecMode = JMV_MEDIA_CODEC_SYNC_MODE;
	videoParams.decMode = JMV_DECODE_COMBINED_MODE;
	// videoParams.decMode = JMV_DECODE_STANDALONE_MODE;

	videoParams.decProfile = 66;

	JmgpuMediaCodec videoDecoder = jmgpuMediaCodecCreate(context, videoParams);

	decode->decode = videoDecoder;
	return decode;
}


void decodeDestory(Decode *decode)
{
    jmgpuMediaCodecDestroy(decode->decode);
}

void decodeStart(Decode *decode)
{
    jmgpuMediaCodecStart(decode->decode);
}
void decodeStop(Decode *decode)
{
    jmgpuMediaCodecStop(decode->decode);
}






int64_t currentTimeStamp(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (int64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int64_t timeToTimeStampMs(struct timeval tv)
{
	return (int64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
