#ifndef __DECODE_H__
#define __DECODE_H__

#include "jmgpu_media_codec.h"
#include <stdlib.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
  JmgpuMediaCodec decode;
} Decode;

Decode* decodeCreate(JmgpuMediaContext context);

int  decodeInit(Decode *decode);
void  decodeRelease(Decode *decode);

void decodeStart(Decode *decode);
void decodeStop(Decode *decode);

int startRtspClientNew(Decode *decode, const char* url);

#ifdef __cplusplus
}
#endif

#endif