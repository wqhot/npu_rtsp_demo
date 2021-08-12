#ifndef __JMGPU_MEDIA_CONTEXT_H__
#define __JMGPU_MEDIA_CONTEXT_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void* JmgpuMediaContext;

JmgpuMediaContext jmgpuMediaContextCreate();
void jmgpuMediaContextDestroy(JmgpuMediaContext context);

#ifdef __cplusplus
}
#endif

#endif