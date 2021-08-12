#include <decode.h>

Decode *decodeCreate(JmgpuMediaContext context, int width, int height, uint32_t decProfile)
{
	Decode *decode = calloc(1, sizeof(Decode));
	JmgpuVideoParams videoParams;
	videoParams.inputFormat = JMV_VIDEO_DATA_FORMAT_H264;
	videoParams.outputFormat = JMV_VIDEO_DATA_FORMAT_R8G8B8A8;
	videoParams.videoWidth = width;
	videoParams.videoHeight = height;
	videoParams.codecMode = JMV_MEDIA_CODEC_ASYNC_MODE;
	// videoParams.codecMode = JMV_MEDIA_CODEC_SYNC_MODE;
	videoParams.decMode = JMV_DECODE_COMBINED_MODE;
	videoParams.decProfile = decProfile;

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