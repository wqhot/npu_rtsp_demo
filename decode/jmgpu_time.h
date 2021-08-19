#ifndef __JMGPU_TIME_H__
#define __JMGPU_TIME_H__


#ifdef __cplusplus
extern "C"
{
#endif


#include <stdint.h>

int64_t jmgpuCurrentTimeStamp(void);

uint64_t jmgpuGetTime(void);

int64_t jmgpuTimeToTimeStampMs(struct timeval tv);

#ifdef __cplusplus
}
#endif

#endif