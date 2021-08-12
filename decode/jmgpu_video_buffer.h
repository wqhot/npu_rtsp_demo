#ifndef __JMGPU_MEDIA_BUFFER_H__
#define __JMGPU_MEDIA_BUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "jmgpu_media_constants.h"
#include "jmgpu_media_context.h"
#include "jmgpu_types.h"

#define JMD_AV_NOPTS_VALUE ((int64_t)UINT64_C(0x8000000000000000))

typedef enum JmgpuVideoDataFormat {
	JMV_VIDEO_DATA_FORMAT_UNKNOWN,

	JMV_VIDEO_DATA_FORMAT_MPEG12, /**< MPEG1, MPEG2 */
	JMV_VIDEO_DATA_FORMAT_MPEG4,  /**< DIVX, XVID */
	JMV_VIDEO_DATA_FORMAT_VC1,	  /**< WMV */
	JMV_VIDEO_DATA_FORMAT_H264,	  /**< H.264 */
	JMV_VIDEO_DATA_FORMAT_HEVC,	  /**< H.265 */

	JMV_VIDEO_DATA_FORMAT_400,
	JMV_VIDEO_DATA_FORMAT_420P,
	JMV_VIDEO_DATA_FORMAT_420SP,
	JMV_VIDEO_DATA_FORMAT_422,
	JMV_VIDEO_DATA_FORMAT_444,
	JMV_VIDEO_DATA_FORMAT_NONE,

	JMV_VIDEO_DATA_FORMAT_B8G8R8A8,
	JMV_VIDEO_DATA_FORMAT_R8G8B8A8,
	JMV_VIDEO_DATA_FORMAT_R10G10B10A2,
	JMV_VIDEO_DATA_FORMAT_B10G10R10A2,
	JMV_VIDEO_DATA_FORMAT_A8
} JmgpuVideoDataFormat;

typedef struct JmgpuVideoBufferInfo {
	JmgpuVideoDataFormat videoDataFormat;
	int width;
	int height;
	s64 pts;
	s64 dts;
} JmgpuVideoBufferInfo;

typedef void *JmgpuVideoBuffer;

JmgpuVideoBuffer jmgpuVideoBufferCreate(JmgpuMediaContext context);
void jmgpuVideoBufferDestory(JmgpuVideoBuffer videoBuffer);

JmgpuMediaStatus jmgpuVideoBufferSetBufferInfo(JmgpuVideoBuffer videoBuffer, JmgpuVideoBufferInfo bufferInfo);
JmgpuMediaStatus jmgpuVideoBufferGetBufferInfo(JmgpuVideoBuffer videoBuffer, JmgpuVideoBufferInfo *bufferInfo);

JmgpuMediaStatus jmgpuVideoBufferRead(JmgpuVideoBuffer videoBuffer, u8 *dstBuffer, int dstBufferSize);
JmgpuMediaStatus jmgpuVideoBufferWrite(JmgpuVideoBuffer videoBuffer, u8 *srcBuffer, int srcBufferSize);

JmgpuMediaStatus jmgpuVideoBufferAllocFb(JmgpuVideoBuffer videoBuffer, int bufferSize);
JmgpuMediaStatus jmgpuVideoBufferFreeFb(JmgpuVideoBuffer videoBuffer);
JmgpuMediaStatus jmgpuVideoBufferReadFb(JmgpuVideoBuffer videoBuffer, u8 *dstBuffer, int dstBufferSize);
JmgpuMediaStatus jmgpuVideoBufferWriteFb(JmgpuVideoBuffer videoBuffer, u8 *srcBuffer, int dstBufferSize);

void *jmgpuVideoBufferGetFbAddr(JmgpuVideoBuffer videoBuffer);

#ifdef __cplusplus
}
#endif

#endif