#ifndef __JMGPU_MEDIA_CODEC_H__
#define __JMGPU_MEDIA_CODEC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "jmgpu_media_constants.h"
#include "jmgpu_video_buffer.h"
#include "jmgpu_media_context.h"
#include "jmgpu_video_state.h"


typedef enum  
{
    JMV_DECODE_STANDALONE_MODE,//解码h264->yuv
    JMV_DECODE_COMBINED_MODE,    //解码h264->rgba
}JmgpuDecodeMode;

typedef enum JmgpuMediaCodecMode {
	JMV_MEDIA_CODEC_SYNC_MODE,	//内部不会开线程
	JMV_MEDIA_CODEC_ASYNC_MODE	//内部开线程解码
} JmgpuMediaCodecMode;

typedef struct JmgpuVideoParams {
	JmgpuVideoDataFormat inputFormat;
	JmgpuVideoDataFormat outputFormat;
	unsigned int videoWidth;
	unsigned int videoHeight;
	JmgpuMediaCodecMode codecMode;
	JmgpuDecodeMode decMode;
	JmgpuDecoderProfile decProfile;
} JmgpuVideoParams;

typedef void* JmgpuMediaCodec;

JmgpuMediaCodec jmgpuMediaCodecCreate(JmgpuMediaContext context, JmgpuVideoParams videoParams);
void jmgpuMediaCodecDestroy(JmgpuMediaCodec mediaCodec);

JmgpuMediaStatus jmgpuMediaCodecStart(JmgpuMediaCodec mediaCodec);
JmgpuMediaStatus jmgpuMediaCodecStop(JmgpuMediaCodec mediaCodec);

JmgpuVideoBuffer jmgpuMediaCodecDequeueInputBuffer(JmgpuMediaCodec mediaCodec, int timeOutMs);
JmgpuMediaStatus jmgpuMediaCodecQueueInputBuffer(JmgpuMediaCodec mediaCodec, JmgpuVideoBuffer mediaBuffer);

JmgpuVideoBuffer jmgpuMediaCodecDequeueOutputBuffer(JmgpuMediaCodec mediaCodec, int timeOutMs);
JmgpuMediaStatus jmgpuMediaCodecReleaseOutputBuffer(JmgpuMediaCodec mediaCodec, JmgpuVideoBuffer mediaBuffer);

#ifdef __cplusplus
}
#endif

#endif