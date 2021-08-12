/**
*******************************************************************************
*           Copyright (C), 2021, ChangSha JingJia Micro Co., Ltd.
*******************************************************************************
* @file          jmgpu_types.h
* @brief         本文件自定义了基本数据类型
* @author        rfshen
* @version       1.0.0
* @date          2021-01-14\n
* @htmlonly 
* <span style="font-weight: bold">更新历史</span> 
* @endhtmlonly
* 版本号|说明|修订者|修订日期
* ------|----|------|--------
* v1.0.0|创建文档|rfshen|2021-01-14
*******************************************************************************/

#ifndef _JMGPU_TYPES_H_
#define _JMGPU_TYPES_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @name 类型定义
 * @{
 */
#ifdef __LP64__
typedef unsigned char      u8;
typedef unsigned int       u32;
typedef unsigned long      u64;
typedef unsigned long long u128;
typedef signed char        s8;
typedef signed int         s32;
typedef signed long        s64;
typedef signed long long   s128;
typedef float              f32;
typedef double             f64;
typedef long double        f128;
#else
typedef unsigned char      u8;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef signed char        s8;
typedef signed int         s32;
typedef signed long long   s64;
typedef float              f32;
typedef double             f64;
#endif
typedef unsigned long      uptr;
typedef signed long        sptr;
/**@} */

/**
 * @name 常量定义
 * @{
 */
#define JMD_TRUE 1
#define JMD_FALSE 0
/**@} */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
