#ifndef __JMGPU_CONCURRENT_QUEUE_H__
#define __JMGPU_CONCURRENT_QUEUE_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*DataFreeFunc)(void *data);
typedef struct JmgpuConcurrentQueue JmgpuConcurrentQueue;

JmgpuConcurrentQueue *jmgpuConcurrentQueueCreate();
void jmgpuConcurrentQueueDestory(JmgpuConcurrentQueue *queue);

void jmgpuConcurrentQueueInit(JmgpuConcurrentQueue *queue);
void jmgpuConcurrentQueueRelease(JmgpuConcurrentQueue *queue);

void jmgpuConcurrentQueueClear(JmgpuConcurrentQueue *queue);
int jmgpuConcurrentQueueSize(JmgpuConcurrentQueue *queue);

void jmgpuConcurrentQueueAddData(JmgpuConcurrentQueue *queue, void *data, DataFreeFunc dataFreeFunc);
void jmgpuConcurrentQueueRemoveData(JmgpuConcurrentQueue *queue, void *data);
int jmgpuConcurrentQueueContainData(JmgpuConcurrentQueue *queue, void *data);
void *jmgpuConcurrentQueuePollFirst(JmgpuConcurrentQueue *queue);
void *jmgpuConcurrentQueueFirst(JmgpuConcurrentQueue *queue);
void *jmgpuConcurrentQueuePeek(JmgpuConcurrentQueue *queue);
void jmgpuConcurrentQueueNotify(JmgpuConcurrentQueue *queue);
void jmgpuConcurrentQueueLock(JmgpuConcurrentQueue *queue);
void jmgpuConcurrentQueueUnLock(JmgpuConcurrentQueue *queue);
void jmgpuConcurrentQueueWait(JmgpuConcurrentQueue *queue);
void jmgpuConcurrentQueueWaitTime(JmgpuConcurrentQueue *queue, int ms);
void jmgpuConcurrentQueueAbort(JmgpuConcurrentQueue *queue);
void jmgpuConcurrentQueueCancerAbort(JmgpuConcurrentQueue *queue);

#ifdef __cplusplus
}
#endif

#endif