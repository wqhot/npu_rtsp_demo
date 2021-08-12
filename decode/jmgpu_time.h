#ifndef __JMGPU_TIME_H__
#define __JMGPU_TIME_H__


#ifdef __cplusplus
extern "C"
{
#endif


#include <stdint.h>

int64_t jmgpuCurrentTimeStamp(void);

uint64_t jmgpuGetTime(void);


#ifdef __cplusplus
}
#endif

#endif