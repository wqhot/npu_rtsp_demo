/**
* @file    mwv206api.h
* @brief   mwv206设备相关的接口函数\n
*          Copyright 2018 长沙景嘉微电子股份有限公司\n
* @author  rfshen
* @version 1.0.0\n
* @htmlonly 
* <span style="font-weight: bold">更新历史</span> 
* @endhtmlonly 
* 版本号|说明|修订者|修订日期 
* ------|----|------|-------- 
* v1.0.0|创建文档|rfshen|2018-06-26 
*/

#ifndef __MWV206API_H
#define __MWV206API_H
#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief 发送GPU 3D模块配置命令到mwv206设备
* @since 1.0.0
* @param dev      图形设备文件句柄,\n
*                 调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                 调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param cmd      mwv206可以接受的命令序列
* @param cmdcount 命令序列个数，单位为32bit命令
* @return 成功发送到mwv206的命令个数
*/
int mwv206DevSendCommand(int dev, unsigned int *cmd, int cmdcount);

/**
* @brief 发送GPU 3D模块配置命令到多个mwv206设备
*        往不同的mwv206设备发送相同的命令
*        用于双显卡交火
* @since 1.0.0
* @param devArr   图形设备文件句柄数组,\n
*                 调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                 调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt   图形设备文件句柄数组中的设备个数
* @param cmd      mwv206可以接受的命令序列
* @param cmdcount 命令序列个数，单位为32bit命令
* @return 返回成功发送到mwv206设备的命令个数，
*         其中一个设备发送不成功返回0，全部发送成功返回cmdcount。
*/
int mwv206DevSendCommandV(int *devArr, int devcnt, unsigned int *cmd, int cmdcount);

/**
* @brief 发送GPU配置命令到CPU内存
* @since 1.0.0
* @param dev      图形设备文件句柄,\n
*                 调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                 调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param cmd      mwv206可以接受的命令序列
* @param cmdcount 命令序列个数，单位为32bit命令
* @return 成功发送到mwv206的命令个数
*/
int mwv206DevWriteCommandToCPUMem(int dev, unsigned int *cmd, int cmdcount);

/**
* @brief 等待图形设备完成3D绘图操作
* @since 1.0.0
* @param dev    图形设备文件句柄,\n
*               调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*               调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param delay  等待时间，这个参数linux内部实现没用，如需长时间等待，请设置等待次数
* @see   ::mwv206SetWaitTimes
* @return
*       -  0 成功
*       - -2 设备访问失败
*/
int mwv206DevWaitForIdle(int dev, int delay);

/**
* @brief 顺序等待多个图形设备完成3D绘图操作
*        用于双显卡交火
* @since 1.0.0
* @param devArr 图形设备文件句柄数组,\n
*               调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*               调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt 图形设备文件句柄数组中的设备个数
* @param delay  等待时间，这个参数linux内部实现没用，如需长时间等待，请设置等待次数
* @see   ::mwv206SetWaitTimes
* @return
*       -  0 成功
*       - -2 设备访问失败
*/
int mwv206DevWaitForIdleV(int *devArr, int devcnt, int delay);







/**
* @brief 设置mwv206设备发送3D命令和等待3D空闲的超时次数
* @since 1.0.0
* @param times 次数，默认10
* @note  主要用于仿真器Z1/ZEBU测试。
*        命令发送和空闲等待的真正处理都在内核驱动，
*        而仿真器降频倍率高，GPU处理速度慢，一个内核最长的等待时间可能无法完成命令，
*        所以设置次数来提升等待时间。等待时间为times*HZ*15
*/
void mwv206SetWaitTimes(int times);

/**
* @brief 发送GPU 2D模块配置命令到mwv206设备
* @since 1.0.0
* @param dev      图形设备文件句柄,\n
*                 调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                 调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param cmd      mwv206可以接受的命令序列
* @param cmdcount 命令序列个数，单位为32bit命令
* @return 成功发送到mwv206的命令个数
*/
int mwv206DevSendCommand2D(int dev, unsigned int *cmd, int cmdcount);

/**
* @brief 单个mwv206设备的寄存器读
* @since 1.0.0
* @param  dev     图形设备文件句柄,\n
*                 调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                 调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param  regaddr 寄存器地址
* @return 返回读取的寄存器的值 
*     - 0xffffffff: 表示读取失败
* @see ::mwv206DevWriteReg
*/
unsigned int mwv206DevReadReg(int dev, int regaddr);

/**
* @brief 单个mwv206设备的寄存器写
* @since 1.0.0
* @param dev     图形设备文件句柄,\n
*                调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param regaddr 寄存器地址
* @param value   寄存的值
* @return 
*     - value:      写入成功
*     - 0xffffffff: 写入失败
* @see ::mwv206DevReadReg
*/
unsigned int mwv206DevWriteReg(int dev, int regaddr, unsigned int value);

/**
* @brief 多个mwv206设备的寄存器写,
*        往不同的mwv206设备的相同寄存器写入相同的值,
*        用于双显卡交火
* @since 1.0.0
* @param devArr  图形设备文件句柄数组,\n
*                调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt  图形设备文件句柄数组中的设备个数
* @param regaddr 寄存器地址
* @param value   寄存的值
* @see ::mwv206DevWriteReg
* @return 
*     - value:      写入成功
*     - 0xffffffff: 写入失败
*/
unsigned int mwv206DevWriteRegV(int *devArr, int devcnt, int regaddr, unsigned int value);

/**
* @name 普通方式访问帧存
* @{
*/

/** 
* @brief 映射mwv206设备的帧存到用户空间
* @since 1.0.0
* @param dev 图形设备文件句柄,\n
*            调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*            调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @note 
*     - 对于非桌面驱动，帧存访问之前必须先调用这个函数
*     - 对于linux系统，需要有root权限
*     - 调用这个函数后，需要调用对应的解除映射函数
* @see ::mwv206MemmapExit ::mwv206MemmapExit2
* @return
*     - 0 成功
*     - -1 创建驱动内部用的hash table映射表失败
*     - -2 没有找到设备
*/
int mwv206MemmapInit(int dev);

/**
* @brief 解除所有已经映射的mwv206设备帧存到用户空间的映射
* @since 1.0.0
* @note  调用mwv206MemmapInit映射的帧存，才能调用这个函数解映射
* @see   ::mwv206MemmapInit
*/
void mwv206MemmapExit(void);

/**
* @brief 申请帧存
* @since 1.0.0
* @param dev   图形设备文件句柄,\n
*              调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*              调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param size  申请大小，字节为单位
* @param align 对齐要求，align要求是2的幂
* @return 返回分配的地址
       - 0 分配失败
* @note 这个地址不能直接访问，必须通过mwv206DevMemWrite/mwvDevMemRead访问
* @see ::mwv206DevMemWrite ::mwv206DevMemRead
*/
unsigned int mwv206DevMemAlloc(int dev, int size, int align);

/**
* @brief 释放帧存
* @since 1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mwv206addr 要释放的帧存地址，必须是mwv206DevMemAlloc返回的值
* @return
*         - 0  成功
*         - -2 设备访问失败
* @see ::mwv206DevMemAlloc
*/
int mwv206DevMemFree(int dev, unsigned int mwv206addr);

/**
* @brief 读取帧存连续地址空间数据到主机内存
* @since 1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mwv206addr 帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param pAddr      主机内存
* @param size       读取数据长度，单位byte
* @return 
*       - 0  成功
*       - -2 设备访问失败
* @see ::mwv206DevMemAlloc
*/
int mwv206DevMemRead(int dev, unsigned int mwv206addr, unsigned char *pAddr, int size);

/**
* @brief 读取帧存块数据到主机内存
* @since 1.0.0
* @param dev          图形设备文件句柄,\n
*                     调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                     调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mwv206addr   帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param mwv206stride 帧存块的stride
* @param pAddr        主机内存地址
* @param stride       主机内存stride
* @param width        要读取的帧存块宽度，单位byte
* @param height       要读取的帧存块高度
* @return 
*       - 0  成功
*       - -2 设备访问失败
* @see ::mwv206DevMemAlloc ::mwv206DevMemWriteBlock
*/
int mwv206DevMemReadBlock(int dev, unsigned int mwv206addr, int mwv206stride, unsigned char *pAddr, int stride, int width, int height);

/**
* @brief 将主机内存数据写入到单个设备帧存连续地址空间
* @since 1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mwv206addr 帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param pAddr      主机内存地址
* @param size       写入数据长度，单位byte
* @return 
*       - 0  成功
*       - -2 设备访问失败
* @see ::mwv206DevMemAlloc ::mwv206DevMemRead
*/
int mwv206DevMemWrite(int dev, unsigned int mwv206addr, const unsigned char *pAddr, int size);
/* 通过DMA通道0，往帧存写数据
 */

/**
* @brief 将主机内存数据写入到多个设备帧存连续地址空间
*        往不同的mwv206设备写入相同的数据
*        用于双显卡交火
* @since 1.0.0
* @param devArr     图形设备文件句柄数组,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt     图形设备文件句柄数组中的设备个数
* @param mwv206addr 帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param pAddr      主机内存地址
* @param size       写入数据长度，单位byte
* @return 
*       -  0 成功
*       - -2 设备访问失败
* @see ::mwv206DevMemAlloc ::mwv206DevMemRead
*/
int mwv206DevMemWriteV(int *devArr, int devcnt, unsigned int mwv206addr,
	const unsigned char *pAddr, int size);

/**
* @brief 将主机内存数据写入到单个设备帧存块
* @since 1.0.0
* @param dev          图形设备文件句柄,\n
*                     调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                     调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mwv206addr   帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param mwv206stride 帧存块的stride
* @param pAddr        主机内存地址
* @param stride       主机内存stride
* @param width        要写入的帧存块宽度，单位byte
* @param height       要写入的帧存块高度
* @return 
*       - 0  成功
*       - -2 设备访问失败
* @see ::mwv206DevMemAlloc ::mwv206DevMemReadBlock
*/
int mwv206DevMemWriteBlock(int dev, unsigned int mwv206addr, int mwv206stride, unsigned char * pAddr, int stride, int width, int height);

/**
* @brief 将主机内存数据写入到多个设备帧存块
*        往不同的mwv206设备写入相同的数据
*        用于双显卡交火
* @since 1.0.0
* @param devArr       图形设备文件句柄数组,\n
*                     调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                     调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt       图形设备文件句柄数组中的设备个数
* @param mwv206addr   帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param mwv206stride 帧存块的stride
* @param pAddr        主机内存地址
* @param stride       主机内存stride
* @param width        要写入的帧存块宽度，单位byte
* @param height       要写入的帧存块高度
* @return
*       -  0 成功
*       - -2 设备访问失败
* @see ::mwv206DevMemAlloc ::mwv206DevMemWriteBlock
*/
int mwv206DevMemWriteBlockV(int *devArr, int devcnt,
	unsigned int mwv206addr, int mwv206stride,
	unsigned char *pAddr, int stride, int width, int height);

/**
* @brief 为单个设备帧存连续地址空间写固定值
* @since 1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mwv206addr 帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param value      要写入的值
* @param vsize      值的宽度（必须是1,2,4之一）
* @param count      访问大小(按照值宽度单位计算)
* @return 
*       -  0 成功
*       - -2 设备访问失败
* @see ::mwv206DevMemAlloc ::mwv206DevMemRead
*/
int mwv206DevMemSet(int dev, unsigned int mwv206addr, unsigned int value, int vsize, int count);

/**
* @brief 将多个设备帧存连续地址空间写固定值
*        往不同的mwv206设备写入相同的数据
*        用于双显卡交火
* @since 1.0.0
* @param devArr     图形设备文件句柄数组,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt     图形设备文件句柄数组中的设备个数
* @param mwv206addr 帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param value      要写入的值
* @param vsize      值的宽度（必须是1,2,4之一）
* @param count      访问大小(按照值宽度单位计算)
* @return 
*       -  0 成功
*       - -2 设备访问失败
* @see ::mwv206DevMemAlloc ::mwv206DevMemRead
*/
int mwv206DevMemSetV(int *devArr, int devcnt,
	unsigned int mwv206addr, unsigned int value, int vsize, int count);

/**  
* @brief  从帧存到帧存的块拷贝 
* @since  1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param srcaddr    帧存源地址，必须是mwv206DevMemAlloc分配的地址
* @param srcstride  源步长，字节为单位
* @param dstaddr    帧存目标地址，必须是mwv206DevMemAlloc分配的地址
* @param dststride  目标步长，字节为单位
* @param width      要拷贝的块长度，字节为单位
* @param height     要拷贝的块高度，像素为单位
* @return 
*       -  0 成功
*       - -2 设备访问失败
* @see ::mwv206DevMemAlloc
*/
int mwv206DevMemCopyBlock(int dev,
	unsigned int srcaddr, int srcstride,
	unsigned int dstaddr, int dststride,
	int width, int height);

/**  
* @brief  从帧存到帧存的连续地址拷贝 
* @since  1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param srcaddr    帧存源地址，必须是mwv206DevMemAlloc分配的地址
* @param dstaddr    帧存目标地址，必须是mwv206DevMemAlloc分配的地址
* @param size       要拷贝的长度，字节为单位
* @return
*       -  0 成功
*       - -2 设备访问失败
* @see ::mwv206DevMemAlloc
*/
int mwv206DevMemCopy(int dev, unsigned int srcaddr, unsigned int dstaddr, int size);

/** 
* @brief 顶点数据发送到GPU后，是否执行帧存检测
* @since 1.0.0
* @param flag
*        - 0: 不检查(默认)
*        - 1: 检查
*/
void mwv206SetVtxFrameCheckFlag(int flag);

/**
* @brief 检测预留帧存地址的读写正确性，\n
* @since 1.0.0
* @param dev 图形设备文件句柄,\n
*            调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*            调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @note
*        由于CPU和GPU是异步执行，CPU发出的写命令返回的时候，GPU内部不一定执行完毕。
*        为了保证之前的写操作完成，可以调用这个函数来执行一次预留帧存地址的写后读检查
* @return
*   -  0 成功
*   - -2 设备访问失败
*/
int mwv206DevMemCheck(int dev);

/**
* @brief 检测多个设备预留帧存地址的读写正确性，\n
*        用于双显卡交火
* @since 1.0.0
* @param devArr  图形设备文件句柄数组,\n
*                调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt  图形设备文件句柄数组中的设备个数
* @see ::mwv206DevMemCheck
* @return
*   -  0 成功
*   - -2 设备访问失败
*/
int mwv206DevMemCheckV(int *devArr, int devcnt);

/**
* @brief 通过2D的专有DMA通道读取帧存数据到主机内存
* @since 1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mwv206addr 要访问的帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param dstaddr    主机内存
* @param size       访问大小，单位byte
* @return 
*       -  0 传输成功
*       - -2 传输失败
* @see  ::mwv206DevMemAlloc ::mwv206Dev2DMemWriteDMA
* @note 各个模块严禁混用DMA通道
*/
int mwv206Dev2DMemReadDMA(int dev, unsigned int mwv206addr,
	unsigned char *dstaddr, int size);

/**
* @brief 通过2D的专有DMA通道，将主机内存数据写入帧存
* @since 1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mwv206addr 要访问的帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param srcaddr    主机内存
* @param size       访问大小，单位byte
* @return 
*       -  0 传输成功
*       - -2 传输失败
* @see  ::mwv206DevMemAlloc ::mwv206Dev2DMemReadDMA
* @note 各个模块严禁混用DMA通道
*/
int mwv206Dev2DMemWriteDMA(int dev, unsigned int mwv206addr,
	const unsigned char *srcaddr, int size);

/**
* @brief 通过3D的专有DMA通道读取帧存数据到主机内存
* @since 1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mwv206addr 要访问的帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param dstaddr    主机内存
* @param size       访问大小，单位byte
* @return 
*       -  0 传输成功
*       - -2 传输失败
* @see  ::mwv206DevMemAlloc ::mwv206Dev3DMemWriteDMA
* @note 各个模块严禁混用DMA通道
*/
int mwv206Dev3DMemReadDMA(int dev, unsigned int mwv206addr,
	unsigned char *dstaddr, int size);

/**
* @brief 通过3D的专有DMA通道，将主机内存数据写入帧存
* @since 1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mwv206addr 要访问的帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param srcaddr    主机内存
* @param size       访问大小，单位byte
* @return 
*       -  0 传输成功
*       - -2 传输失败
* @see  ::mwv206DevMemAlloc ::mwv206Dev3DMemReadDMA
* @note 各个模块严禁混用DMA通道
*/
int mwv206Dev3DMemWriteDMA(int dev, unsigned int mwv206addr,
	const unsigned char *srcaddr, int size);

/**
* @brief 通过2D的专有DMA通道读取2D帧存块数据到主机内存
* @since 1.0.0
* @param dev          图形设备文件句柄,\n
*                     调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                     调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mwv206addr   帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param mwv206stride 帧存块的stride
* @param dstaddr      主机内存地址
* @param stride       主机内存stride
* @param width        要读取的帧存块宽度，单位byte
* @param height       要读取的帧存块高度
* @return 
*       -  0 传输成功
*       - -2 传输失败
* @see ::mwv206DevMemAlloc ::mwv206Dev2DMemWriteBlockDMA
* @note  各个模块严禁混用DMA通道
*/
int mwv206Dev2DMemReadBlockDMA(int dev,
	unsigned int mwv206addr, int mwv206stride,
	unsigned char *dstaddr, int stride,
	int width, int height);

/**
* @brief 通过2D的专有DMA通道，将主机内存数据写入到单个设备帧存块
* @since 1.0.0
* @param dev          图形设备文件句柄,\n
*                     调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                     调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mwv206addr   帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param mwv206stride 帧存块的stride
* @param srcaddr      主机内存地址
* @param stride       主机内存stride
* @param width        要写入的帧存块宽度，单位byte
* @param height       要写入的帧存块高度
* @return 
*       -  0 传输成功
*       - -2 传输失败
* @see ::mwv206DevMemAlloc ::mwv206Dev2DMemReadBlockDMA
* @note  各个模块严禁混用DMA通道
*/
int mwv206Dev2DMemWriteBlockDMA(int dev,
	unsigned int mwv206addr, int mwv206stride,
	unsigned char *srcaddr, int stride,
	int width, int height);

/**
* @brief 通过3D的专有DMA链表通道读取3D帧存块数据到主机内存
* @since 1.0.0
* @param dev          图形设备文件句柄,\n
*                     调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                     调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mwv206addr   帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param mwv206stride 帧存块的stride
* @param dstaddr      主机内存地址
* @param stride       主机内存stride
* @param width        要读取的帧存块宽度，单位byte
* @param height       要读取的帧存块高度
* @return 
*       -  0 传输成功
*       - -2 传输失败
* @see ::mwv206DevMemAlloc ::mwv206Dev3DMemWriteBlockDMA
* @note  各个模块严禁混用DMA通道
*/
int mwv206Dev3DMemReadBlockDMA(int dev,
	unsigned int mwv206addr, int mwv206stride, unsigned char *dstaddr, int stride,
	int width, int height);

/**
* @brief 通过3D的专有DMA链表通道，将主机内存数据写入到单个设备帧存块
* @since 1.0.0
* @param dev          图形设备文件句柄,\n
*                     调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                     调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mwv206addr   帧存地址，必须是mwv206DevMemAlloc分配的地址
* @param mwv206stride 帧存块的stride
* @param srcaddr      主机内存地址
* @param stride       主机内存stride
* @param width        要写入的帧存块宽度，单位byte
* @param height       要写入的帧存块高度
* @return 
*       -  0 传输成功
*       - -2 传输失败
* @see ::mwv206DevMemAlloc ::mwv206Dev3DMemReadBlockDMA
* @note  各个模块严禁混用DMA通道
*/
int mwv206Dev3DMemWriteBlockDMA(int dev,
	unsigned int mwv206addr, int mwv206stride, unsigned char *srcaddr, int stride,
	int width, int height);


/** @} */

/**
* @name 中断模块接口
* @{
*/
 
/**
* @brief 顺序软复位多个设备的3D模块
*        用于双显卡交火
* @since 1.0.0
* @param devArr 图形设备文件句柄数组,\n
*               调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*               调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt 图形设备文件句柄数组中的设备个数
*/
int mwv206DevSoftwareReset3DV(int *devArr, int devcnt);

/** mwv206中断枚举变量*/
typedef enum {
	MWV206INTR_SD,                 /**< 0: SD 卡中断*/
	MWV206INTR_HD_DECODE,          /**< 1: 高清解码模块中断*/
	MWV206INTR_RENDER,             /**< 2: 3D绘图模块中断*/
	MWV206INTR_CMDPORT,            /**< 3: 2D&3D命令解析模块中断*/
	MWV206INTR_CMDSTATUS,          /**< 4: 2D&3D命令解析模块状态中断*/
	MWV206INTR_VS,                 /**< 5: 3D绘图VS中断*/
	MWV206INTR_PS0,                /**< 6: 3D绘图PS像素生成单元0路中断*/
	MWV206INTR_PS1,                /**< 7: 3D绘图PS像素生成单元1路中断*/
	MWV206INTR_PS2,                /**< 8: 3D绘图PS像素生成单元2路中断*/
	MWV206INTR_PS3,                /**< 9: 3D绘图PS像素生成单元3路中断*/
	MWV206INTR_RASTER0,            /**< 10: 3D绘图PS光栅化单元0路中断*/
	MWV206INTR_RASTER1,            /**< 11: 3D绘图PS光栅化单元1路中断*/
	MWV206INTR_RASTER2,            /**< 12: 3D绘图PS光栅化单元2路中断*/
	MWV206INTR_RASTER3,            /**< 13: 3D绘图PS光栅化单元3路中断*/
	MWV206INTR_DISPLAY0,           /**< 14: 视频叠加模块0路场同步中段*/
	MWV206INTR_DISPLAY1,           /**< 15: 视频叠加模块1路场同步中段*/
	MWV206INTR_DISPLAY2,           /**< 16: 视频叠加模块2路场同步中段*/
	MWV206INTR_DISPLAY3,           /**< 17: 视频叠加模块3路场同步中段*/
	MWV206INTR_GRAPH_ABNORMAL,     /**< 18: 视频叠加模块图形异常中断*/
	MWV206INTR_PCIEX16_DMA = 28,   /**< 28: PCIEX16异常合并中断*/
	MWV206INTR_PCIEX4_1_DMA = 33,  /**< 33: PCIEX4_1异常合并中断*/
	MWV206INTR_PCIEX8_DMA = 38,    /**< 34: PCIEX8异常合并中断*/
	MWV206INTR_PCIEX4_0_DMA = 43,  /**< 43: PCIEX4_0异常合并中断*/   
	MWV206INTR_JJWLINK_SLAVE = 68, /**< 68: JJWLINK发送完成中断*/
	MWV206INTR_JJWLINK_MASTER = 69,/**< 69: JJWLINKj接收完成中断*/  
	MWV206INTR_COUNT = 96,         /**< 96: mwv206中断个数*/  
} e_mwv206Intr;

/** mwv206中断分为3个组*/
#define MWV206INTR_GROUP               3

/** mwv206 2D命令口中断事件编号*/
#define MWV206INTR_CMDPORT_2D         (MWV206INTR_COUNT + 8)

/**
* @brief 允许/禁止中断产生，默认路由到PCIE-X8
* @since 1.0.0
* @param dev     图形设备文件句柄,\n
*                调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param intrsrc 中断源ID，见e_mwv206Intr
* @param enable 
*        - 0 禁止
*        - 1 允许
* @see ::e_mwv206Intr
* @return
*        -  0 成功
*        - -1 intrsrc参数错误
*        - -3 设备访问失败
*/
int mwv206DevEnableIntrSrc(int dev, e_mwv206Intr intrsrc, int enable);

/**
* @brief 顺序允许/禁止多个设备的中断产生
*        用于双显卡交火
* @since 1.0.0
* @param devArr  图形设备文件句柄数组,\n
*                调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt  图形设备文件句柄数组中的设备个数
* @param intrsrc 中断源ID，见e_mwv206Intr
* @note 
*        - soc模式，默认路由到内部CPU
*        - EP模式，默认路由到PCIE-X16
* @param enable 
*        - 0 禁止
*        - 1 允许
* @see ::e_mwv206Intr
* @return
*        -  0 成功
*        - -1 intrsrc参数错误
*        - -3 设备访问失败
*/
int mwv206DevEnableIntrSrcV(int *devArr, int devcnt, e_mwv206Intr intrsrc, int enable);

/** mwv206中断路由枚举变量*/
typedef enum {
	MWV206INTR_ROUTE_TO_CPU,      /**< 0: 中断路由到内部CPU*/
	MWV206INTR_ROUTE_TO_PCIE,     /**< 1: 中断路由到PCIE*/
	MWV206INTR_ROUTE_TO_CPU_PCIE, /**< 2: 中断路由到内部CPU和PCIE*/
	MWV206INTR_ROUTE_TO_OUTSIDE,  /**< 3: 中断路由到片外*/
	MWV206INTR_ROUTE_MAX
} e_mwv206IntrRoute;

/**
* @brief 单个中断源的路由选择，在mwv206DevEnableIntrSrc函数之后调用
* @since 1.0.0
* @param dev     图形设备文件句柄,\n
*                调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param intrsrc 中断源ID，见e_mwv206Intr
* @param select  路由选择，见e_mwv206IntrRoute\n
*              - soc模式，默认值为MWV206INTR_ROUTE_TO_CPU
*              - EP模式，默认值为MWV206INTR_ROUTE_TO_PCIE
* @see ::e_mwv206Intr ::e_mwv206IntrRoute
* @return
*        -  0 成功
*        - -1 intrsc参数错误
*        - -2 select参数错误
*        - -3 设备访问失败
*/
int mwv206DevIntrRouteSelect(int dev, e_mwv206Intr intrsrc, e_mwv206IntrRoute select);

/**
* @brief 多个设备的单个中断源的路由选择，在mwv206DevEnableIntrSrc函数之后调用
*        用于双显卡交火
* @since 1.0.0
* @param devArr  图形设备文件句柄数组,\n
*                调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt  图形设备文件句柄数组中的设备个数
* @param intrsrc 中断源ID，见e_mwv206Intr
* @param select  路由选择，见e_mwv206IntrRoute\n
*              - soc模式，默认值为MWV206INTR_ROUTE_TO_CPU
*              - EP模式，默认值为MWV206INTR_ROUTE_TO_PCIE
* @see ::e_mwv206Intr ::e_mwv206IntrRoute
* @return
*        -  0 成功
*        - -1 intrsc参数错误
*        - -2 select参数错误
*        - -3 设备访问失败
*/
int mwv206DevIntrRouteSelectV(int *devArr, int devcnt, e_mwv206Intr intrsrc, e_mwv206IntrRoute select);

/**
* @brief 所有中断源的路由选择，在mwv206DevEnableIntrSrc函数之后调用
* @since 1.0.0
* @param dev     图形设备文件句柄,\n
*                调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param select  路由选择，见e_mwv206IntrRoute\n
*              - soc模式，默认值为MWV206INTR_ROUTE_TO_CPU
*              - EP模式，默认值为MWV206INTR_ROUTE_TO_PCIE
* @see ::e_mwv206Intr ::e_mwv206IntrRoute
* @return
*        -  0 成功
*        - -2 select参数错误
*        - -3 设备访问失败
*/
int mwv206DevIntrRouteSelectAll(int dev, e_mwv206IntrRoute select);

/** mwv206中断路由到PCIE枚举类型*/
typedef enum  {
	MWV206INTR_ROUTE_PCIE_X16,   /**< 0: 中断路由到PCIE_X16*/
	MWV206INTR_ROUTE_PCIE_X4_1,  /**< 1: 中断路由到PCIE_X4_0*/
	MWV206INTR_ROUTE_PCIE_X8,    /**< 2: 中断路由到PCIE_X8*/
	MWV206INTR_ROUTE_PCIE_X4_0,  /**< 3: 中断路由到PCIE_X4_1*/
	MWV206INTR_ROUTE_PCIE_MAX 
} e_mwv206IntrRoutePcie;

/**
* @brief 单个中断源的路由到PCIE选择，在mwv206DevEnableIntrSrc函数之后调用
* @since 1.0.0
* @param dev     图形设备文件句柄,\n
*                调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param intrsrc 中断源ID，见e_mwv206Intr
* @param select  路由选择，见e_mwv206IntrRoutePcie，默认值为MWV206INTR_ROUTE_PCIE_X16
* @see ::e_mwv206Intr ::e_mwv206IntrRoutePcie
* @return
*        -  0 成功
*        - -1 intrsc参数错误
*        - -2 select参数错误
*        - -3 设备访问失败
*/
int mwv206DevIntrRoutePCIESelect(int dev, e_mwv206Intr intrsrc, e_mwv206IntrRoutePcie select);

/**
* @brief 多个设备的单个中断源的路由到PCIE选择，在mwv206DevEnableIntrSrc函数之后调用
*        用于双显卡交火
* @since 1.0.0
* @param devArr  图形设备文件句柄数组,\n
*                调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt  图形设备文件句柄数组中的设备个数
* @param intrsrc 中断源ID，见e_mwv206Intr
* @param select  路由选择，见e_mwv206IntrRoutePcie，默认值为MWV206INTR_ROUTE_PCIE_X16
* @see ::e_mwv206Intr ::e_mwv206IntrRoutePcie
* @return
*        -  0 成功
*        - -1 intrsc参数错误
*        - -2 select参数错误
*        - -3 设备访问失败
*/
int mwv206DevIntrRoutePCIESelectV(int *devArr, int devcnt, e_mwv206Intr intrsrc, e_mwv206IntrRoutePcie select);

/**
* @brief 所有中断源的路由到PCIE选择，在mwv206DevEnableIntrSrc函数之后调用
* @since 1.0.0
* @param dev     图形设备文件句柄,\n
*                调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param select  路由选择，见e_mwv206IntrRoutePcie，默认值为MWV206INTR_ROUTE_PCIE_X16
* @see ::e_mwv206Intr ::e_mwv206IntrRoutePcie
* @return
*        -  0 成功
*        - -2 select参数错误
*        - -3 设备访问失败
*/
int mwv206DevIntrRoutePCIEAll(int dev, e_mwv206IntrRoutePcie select);
/** @} */


/**绘图命令发送模式*/
typedef enum {
	MWV206_SENDCMDTO_CMDPORT,      /**< 0: 发送到命令口*/
	MWV206_SENDCMDTO_FRAMEBUFFER,  /**< 1: 发送到GPU帧存，由GPU主动获取*/
	MWV206_SENDCMDTO_CPUMEM,       /**< 2: 发送到CPU内存，由GPU主动获取*/
	MWV206_SENDCMDTO_MAXVAL,       /**< 3: 命令发送模式类别数目*/
} e_mwv206SendCmdMode;

/**
* @brief 设置绘图命令发送模式，ringbuffer的值为默认值4Mbytes
* @since 1.0.0
* @param dev  图形设备文件句柄,\n
*             调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*             调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mode 命令发送模式，见e_mwv206SendCmdMode
* @see ::e_mwv206SendCmdMode
* @return
*	-  0 成功
*	- -1 参数错误
*	- -2 失败
*/
int mwv206DevSetSendCommandMode(int dev, e_mwv206SendCmdMode mode);

/**
* @brief 顺序设置多个设备的命令发送模式，ringbuffer的值为默认值4Mbytes
*        用于双显卡交火，主从设备需设置一样的模式
* @since 1.0.0
* @param devArr  图形设备文件句柄数组,\n
*                调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt  图形设备文件句柄数组中的设备个数
* @param mode    命令发送模式，见e_mwv206SendCmdMode
* @see ::e_mwv206SendCmdMode
* @return
*	-  0 成功
*	- -1 参数错误
*	- -2 失败
*/
int mwv206DevSetSendCommandModeV(int *devArr, int devcnt, e_mwv206SendCmdMode mode);

/**
* @brief 设置2D驱动绘图命令发送模式，ringbuffer的值为默认值*Mbytes
* @since 1.0.0
* @param dev  图形设备文件句柄,\n
*             调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*             调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mode 命令发送模式，见e_mwv206SendCmdMode
* @see ::e_mwv206SendCmdMode
* @return
*	-  0 成功
*	- -1 参数错误
*	- -2 失败
*/
int mwv206DevSetSendCommandMode2D(int dev,	e_mwv206SendCmdMode mode);

/** 
* @brief 设置绘图命令发送模式和ringbuffer的值
* @since 1.0.0
* @param dev            图形设备文件句柄,\n
*                       调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                       调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param mode           命令发送模式，见e_mwv206SendCmdMode
* @param ringbuffersize 帧存或者内存的大小，单位Mbytes
* @see ::e_mwv206SendCmdMode
* @return
*	-  0 成功
*	- -1 参数错误
*	- -2 失败
*/
int mwv206DevSetSendCommandMode2(int dev, e_mwv206SendCmdMode mode, unsigned int ringbuffersize);

/** 
* @brief 为多个设备设置命令发送模式和ringbuffer的值
*        用于双显卡交火，主从设备需设置一样的模式
* @since 1.0.0
* @param devArr         图形设备文件句柄数组,\n
*                       调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                       调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt         图形设备文件句柄数组中的设备个数
* @param mode           命令发送模式，见e_mwv206SendCmdMode
* @param ringbuffersize 帧存或者内存的大小，单位Mbytes
* @return
*	-  0 成功
*	- -1 参数错误
*	- -2 失败
*/
int mwv206DevSetSendCommandMode2V(int *devArr, int devcnt,
	e_mwv206SendCmdMode mode, unsigned int ringbuffersize);


/** 顶点数据组织模式枚举类型*/
typedef enum {
	MWV206_VERTEX_OUTOFORDER,    /**<0 顶点乱序模式*/
	MWV206_VERTEX_INORDER,       /**<1 顶点顺序模式*/
	MWV206_VERTEXORDER_MAXVAL,   /**<2 顶点模式类别数目*/
} e_mwv206VertexOrder;

/** 
* @brief 设置顶点数据组织模式
* @since 1.0.0
* @param dev   图形设备文件句柄,\n
*              调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*              调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param order 顶点数据组织模式，见e_mwv206VertexOrder
* @see ::e_mwv206VertexOrder
* @return
*	-  0 成功
*	- -1 参数错误
*	- -2 失败
*/
int mwv206DevSetVertexOrder(int dev, e_mwv206VertexOrder order);

/** 
* @brief 为多个设备设置顶点数据组织模式
*        用于双显卡交火，主从设备需设置一样的模式
* @since 1.0.0
* @param devArr 图形设备文件句柄数组,\n
*               调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*               调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt 图形设备文件句柄数组中的设备个数
* @param order  顶点数据组织模式，见e_mwv206VertexOrder
* @see ::e_mwv206VertexOrder
* @return
*	-  0 成功
*	- -1 参数错误
*	- -2 失败
*/
int mwv206DevSetVertexOrderV(int *devArr, int devcnt, e_mwv206VertexOrder order);


/** 顶点数据存放位置枚举类型*/
typedef enum {
	MWV206_VERTEXLOC_GPUMEM,          /**< 0: 顶点数据放在GPU帧存，直接访问*/
	MWV206_VERTEXLOC_CPUMEM,          /**< 1: 顶点数据放在CPU内存*/
	MWV206_VERTEXLOC_GPUMEM_INDIRECT, /**< 2: 顶点数据放在GPU帧存，通过AXI间接访问*/
	MWV206_VERTEXLOC_MAXVAL,          /**< 3: 存放位置的类别数目*/
} e_mwv206VertexLoc;

/** 
* @brief 设置顶点数据存储位置
* @since 1.0.0
* @param dev      图形设备文件句柄,\n
*                 调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                 调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param location 顶点数据存放位置，见e_mwv206VertexLoc
* @see ::e_mwv206VertexLoc
* @return
*	-  0 成功
*	- -1 参数错误
*	- -2 失败
*/
int mwv206DevSetVertexLocation(int dev, e_mwv206VertexLoc location);

/** 
* @brief 为多个设备设置顶点数据存储位置
*        用于双显卡交火，主从设备需设置一样的值
* @since 1.0.0
* @param devArr   图形设备文件句柄数组,\n
*                 调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                 调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param devcnt   图形设备文件句柄数组中的设备个数
* @param location 顶点数据存放位置，见e_mwv206VertexLoc
* @see ::e_mwv206VertexLoc
* @return
*	-  0 成功
*	- -1 参数错误
*	- -2 失败
*/
int mwv206DevSetVertexLocationV(int *devArr, int devcnt, e_mwv206VertexLoc location);

/** 
* @brief 使能同一个图元使用多种光照材料功能,\n
* @since 1.0.0
* @note  同一图元最多只能有16种不同的材料,\n
*        使能后，会降低性能，因为内部变成立即渲染模式
*/
void mwv206EnableVariedMaterial(void);

/** 
* @brief 关闭同一个图元使用多种光照材料功能(默认)
* @since 1.0.0
*/
void mwv206DisableVariedMaterial(void);

/**
* @brief 设置多边形偏移实现相关的r值，默认值是5000.0f
* @since 1.0.0
* @param rval 要设置的r值
*/
void mwv206SetPolygonOffsetRVal(float rval);

/**
* @brief 解除单个mwv206设备帧存到用户空间的映射
* @since 1.0.0
* @param dev 图形设备文件句柄,\n
*            调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*            调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @note  调用mwv206MemmapInit映射的帧存，才能调用这个函数解映射
* @see   ::mwv206MemmapInit
* @return
*    -  0 成功
*    - -1 设备还未映射帧存
*/
int mwv206MemmapExit2(int dev);

/**
* @brief 锁定图形设备，保证对mwv206的绘图操作不被其他任务中断
* @since 1.0.0
* @param dev   图形设备文件句柄,\n
*              调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*              调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param lock
*        - 0 解除锁定
*        - 1 锁定
* @return
*        -  0 成功
*        - -1 参数错误
*        - -2 设备访问失败
*/
int mwv206DevLock(int dev, int lock);

/**
* @brief 关闭VS和FS
* @since 1.0.0
* @param dev  图形设备文件句柄,\n
*             调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*             调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @return 
*     - 0:  成功
*     - -1: 失败
*/
int mwv206DevDisableShader(int dev);

/**
* @brief 配置锁相环
* @since 1.0.0
* @param dev   图形设备文件句柄,\n
*              调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*              调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param pllID 锁相环的id号
* @param freq  锁相环的配置频率值
* @return
*     - 0  成功
*     - -1 参数错误
*     - -2 分频参数计算错误
*     - -3 等待锁相环锁定超时
*/
int mwv206ConfigPll(int dev, int pllID, unsigned int freq);

/** 获取mwv206设备内部状态的枚举类型*/
typedef enum {
	MWV206PARAM_RESMEMSIZE,       /**< 0: 预留帧存大小，单位?*/
	MWV206PARAM_CORECLOCK,        /**< 1: 时钟频率*/
	MWV206PARAM_MEMCLOCK,         /**< 2: 帧存时钟频率*/
	MWV206PARAM_MEMFREE,          /**< 3: 剩余帧存大小，单位?*/
	MWV206PARAM_IRQ,              /**< 4: IRQ?中断个数*/
	MWV206PARAM_ISPOWERDOWN,      /**< 5: 是否关机?*/
	MWV206PARAM_TILEADDRESS,      /**< 6: tile address*/
	MWV206PARAM_SENDCMDMODE,      /**< 7: 命令发送模式*/
	MWV206PARAM_VERTEXORDER,      /**< 8: 顶点数据组织模式*/
	MWV206PARAM_VERTEXLOCATION,   /**< 9: 顶点数据存放位置*/
	MWV206PARAM_MEMBAR_BASEADDR,  /**< 10: 内核映射的帧存起始地址*/
	MWV206PARAM_MEMBLOCKSIZE,     /**< 11: 帧存管理的分块大小*/
	MWV206PARAM_DDRCNT,           /**< 12: 帧存DDR的个数*/
	MWV206PARAM_BARNO,            /**< 13: 设备bar编号*/
	MWV206PARAM_BUSNO,            /**< 14: 设备对应的总线编号*/
	MWV206PARAM_RBSIZE,           /**< 15: mwv206主动获取命令模式的ringbuffer大小*/
	MWV206PARAM_DDR0USERCHECKADDR,    /**< 16: 预留的用于用户空间校验ddr0的地址*/
	MWV206PARAM_DDR1USERCHECKADDR,    /**< 17: 预留的用于用户空间校验ddr1的地址*/
} e_mwv206Param;
	

/**
* @brief 获取mwv206设备相关的参数
* @since 1.0.0
* @param dev      图形设备文件句柄,\n
*                 调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                 调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param  paramid 设备参数id，取值见e_mwv206Param
* @return 获取的设备参数值
* @see ::e_mwv206Param
*/
unsigned int mwv206DevGetDevParam(int dev, e_mwv206Param paramid);

/**
* @brief 获取mwv206设备预留帧存大小
* @since 1.0.0
* @param dev  图形设备文件句柄,\n
*             调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*             调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @return mwv206设备预留帧存大小
*/
unsigned int mwv206DevGetResMemSize(int dev);

/**
* @brief 判断mwv206设备是否关机状态
* @since 1.0.0
* @param dev 图形设备文件句柄,\n
*            调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*            调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @return
*       - 0 关机
*       - 1 开机
*/
unsigned int mwv206DevIsPowerDown(int dev);

/**
* @brief 设置mwv206设备关机状态
* @since 1.0.0
* @param dev 图形设备文件句柄,\n
*            调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*            调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param powerdown
*       - 0  进入低功耗状态，先复位设备，再关闭时钟
*       - 1  恢复正常，先打开时钟，再取消复位
* @return
*       -  0 成功
*       - -2 设备访问失败
*/
int mwv206DevSetPowerDown(int dev, int powerdown);

/**
* @brief 软复位3D模块
* @since 1.0.0
* @param dev 图形设备文件句柄,\n
*            调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*            调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
*/
int mwv206DevSoftwareReset3D(int dev);

/** mwv206内部模块枚举类型*/
typedef enum {
	MWV206_AXI_INTERNAL, /**< 0: AXI互联模块的内部转换模块*/
	MWV206_AXI_APB,      /**< 1: AXI互联模块的APB输出总线模块*/
	MWV206_AXI_JJWLB,    /**< 2: 芯片内部JJWLB本地总线模块*/
	MWV206_AXI_DDR,      /**< 3: DDR模块*/
	MWV206_CMDPORT,      /**< 4: 命令口模块*/
	MWV206_2D,           /**< 5: 绘图模块2D单元*/
	MWV206_3D,           /**< 6: 绘图模块3D单元*/
	MWV206_HD,           /**< 7: 高清解码模块*/
	MWV206_OTP,          /**< 8: OTP模块*/
	MWV206_RAM,          /**< 9: RAM单元模块*/
	MWV206_ROM,          /**< 10: ROM单元模块*/
	MWV206_SDEMMC,       /**< 11: SD_EMMC卡模块*/
	MWV206_VBIOS,        /**< 12: VBIOS模块*/
	MWV206_EthNet0,      /**< 13: 0号以太网模块*/
	MWV206_EthNet1,      /**< 14: 1号以太网模块*/
	MWV206_DAC0,         /**< 15: 0号DAC输出模块*/
	MWV206_DAC1,         /**< 16: 1号DAC输出模块*/
	MWV206_DVO0,         /**< 17: 0号DVO输出模块*/
	MWV206_DVO1,         /**< 18: 1号DVO输出模块*/
	MWV206_LVDS0,        /**< 19: 0号LVDS输出模块*/
	MWV206_LVDS1,        /**< 20: 1号LVDS输出模块*/
	MWV206_HDMI0,        /**< 21: 0号HDMI输出模块*/
	MWV206_HDMI1,        /**< 22: 1号HDMI输出模块*/
	MWV206_HDMI2,        /**< 23: 2号HDMI输出模块*/
	MWV206_HDMI3,        /**< 24: 3号HDMI输出模块*/
	MWV206_EXTVIDEO_CHK, /**< 25: 外视频检测模块*/
	MWV206_VIDEO_ZOOM,   /**< 26: 视频缩放模块*/
	MWV206_GRAPH_OUT0,   /**< 27: 0号图形输出模块*/
	MWV206_GRAPH_OUT1,   /**< 28: 1号图形输出模块*/
	MWV206_GRAPH_OUT2,   /**< 29: 2号图形输出模块*/
	MWV206_GRAPH_OUT3,   /**< 30: 3号图形输出模块*/
	MWV206_EXTVIDEO_IN0, /**< 31: 0号外视频输入模块*/
	MWV206_EXTVIDEO_IN1, /**< 32: 1号外视频输入模块*/
	MWV206_EXTVIDEO_IN2, /**< 33: 2号外视频输入模块*/
	MWV206_EXTVIDEO_IN3, /**< 34: 3号外视频输入模块*/
	MWV206_CPU,          /**< 35: CPU模块*/
} e_mwv206Module;

/**
* @brief mwv206内部模块时钟关断，用于降低功耗
* @since 1.0.0
* @param dev 图形设备文件句柄,\n
*            调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*            调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param module 模块ID，见e_mwv206Module
* @param on 
*       - 0 关闭模块时钟
*       - 1 开启模块时钟
* @see ::e_mwv206Module
* @return
*   -  0 成功
*   - -1 module参数错误
*   - -2 设备访问失败
* TODO: 未实现
*/
int mwv206DevSetClock(int dev, e_mwv206Module module, int on);

/**
* @brief 关闭VBIOS模块
* @since 1.0.0
* @param dev 图形设备文件句柄,\n
*            调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*            调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @return
*   -  0 成功
*   - -2 设备访问失败
*/
int mwv206DevCloseStartupScreen(int dev);

/**
* @name 内核内存访问接口
* @{
*/

/**
* @brief 申请内核内存
* @since 1.0.0
* @param dev   图形设备文件句柄,\n
*              调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*              调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param size  申请大小，字节为单位
* @return 返回分配的地址
*	 - 0 失败
* @note  这个地址不能直接访问，必须通过mwv206CPUMemRead/mwv206CPUMemWrite访问
* @see  ::mwv206CPUMemRead ::mwv206CPUMemWrite
*/
unsigned long mwv206CPUMemAlloc(int dev, int size);

/**
* @brief 释放内核内存
* @since 1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param kernelAddr 要释放的内核内存地址，必须是mwv206CPUMemAlloc返回的值
* @see ::mwv206CPUMemAlloc
* @return
*	-  0 成功
*	- -2 设备访问失败
*/
int mwv206CPUMemFree(int dev, unsigned long kernelAddr);

/**
* @brief 申请内核内存，且内存不经过DATA Cache
* @since 1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param dma_handle 分配的内存dma操作地址
* @param size       申请的大小，字节为单位
* @return           分配的内存虚拟地址
*/
unsigned long mwv206CPUDMAMemAlloc(int dev, int size, unsigned long *dma_handle);

/**
* @brief 释放mwv206CPUDMAMemAlloc分配的内核内存
* @since 1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param vaddr      要释放的内存虚拟地址
* @param dma_handle 要释放的内存dma操作地址
* @param size       要释放的内存大小，字节为单位
* @return
*	-  0 成功
*	- -2 失败
*/
int mwv206CPUDMAMemFree(int dev,
	unsigned long vaddr, unsigned long dma_handle, int size);

/**  
* @brief  mwv206DevDecMemRead 高清解码程序读帧存
* @since  1.0.0
* @param[in] dev		图形设备文件句柄,\n
*                   	调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   	调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param[in] mwv206addr 帧存地址
* @param[in] pAddr		读取数据的输出地址
* @param[in] size		读取大小
* @return 
*   -  0 成功
* 	-  非0 失败
* @note 
* @see   
* @author wq
* @date   2018/7/20
*/
int mwv206DevDecMemRead(int dev, unsigned int mwv206addr, unsigned char *pAddr, int size);

/**  
* @brief  mwv206DevDecMemWrite 高清解码程序写帧存
* @since  1.0.0
* @param[in] dev		图形设备文件句柄,\n
*                   	调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   	调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param[in] mwv206addr	帧存地址
* @param[in] pAddr		写入数据的输出地址
* @param[in] size		写入大小
* @return 
*   -  0 成功
* 	-  非0 失败
* @note 
* @see   
* @author wq
* @date   2018/7/20
*/
int mwv206DevDecMemWrite(int dev, unsigned int mwv206addr, unsigned char *pAddr, int size);



/**
* @brief 将内核地址空间数据读出到用户地址空间
* @since 1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param kernelAddr 内核空间地址
* @param userAddr   用户空间地址
* @param size       读取长度，以字节为单位
* @return
*	-  0 成功
*	- -2 失败
*/
int mwv206CPUMemRead(int dev, unsigned long kernelAddr,
	unsigned char *userAddr, int size);

/**
* @brief 将用户地址空间数据写入到内核地址空间
* @since 1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param kernelAddr 内核空间地址
* @param userAddr   用户空间地址
* @param size       写入长度，以字节为单位
* @return
*	-  0 成功
*	- -2 失败
*/
int mwv206CPUMemWrite(int dev, unsigned long kernelAddr,
	const unsigned char *userAddr, int size);

/**
* @brief 获取内核地址的物理地址
* @since 1.0.0
* @param dev        图形设备文件句柄,\n
*                   调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                   调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @param kernelAddr 内核空间地址
* @return 物理地址
*/
unsigned long mwv206CPUMemGetPhyAddr(int dev, unsigned long kernelAddr);

/**
* @brief  获取mwv206基础接口库libmwv206api.so的版本信息
* @since  1.0.0
* @return 动态库libmwv206api.so的版本信息
*/
const char *mwv206apiVer(void);

/**
* @brief  获取mwv206内核库mwv206.ko的版本信息
* @since  1.0.0
* @param  dev    图形设备文件句柄,\n
*                调用open("/dev/mwv206_0", O_RDWR), 可以返回第一个图形设备的句柄\n
*                调用open("/dev/mwv206_1", O_RDWR), 可以返回第二个图形设备的句柄
* @return 内核mwv206.ko的版本信息
*/
const char *mwv206kernelVer(int dev);

/**
* @brief 检查主机CPU是否为小端
* @return
*     - 0 小端
*     - 1 大端
*/
int iscpuLE(void);

/**  
* @brief  获取当前时间戳字符串，格式YYYYMMDD.HHMM, 例如20180823.1055
* @since  1.0.0
* @return 时间戳格式字符串
*/
const char *getTimeStampStr(void);

#ifdef __cplusplus
}
#endif

#endif
