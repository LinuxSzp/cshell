#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

/*************************************************************************
    > File Name: rb.h
    > Author: linux
    > Mail: linuxszp@icloud.com 
    > Created Time: 2015年06月08日 星期一 17时51分23秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct _ring_buffer_t {
    uint32_t    block;
    uint32_t    size;
    uint32_t    r;
    uint32_t    w;
    uint8_t     *buf[0];
}ring_buffer_t;

/*
-----------------
|     block     |       //整个缓冲区块的个数
-----------------
|     size      |       //每个块的大小
-----------------
|     r         |       //当前读下标的位置
-----------------
|     w         |       //当前写下标的位置
-----------------
|     *buf[0]   |       //存放数据的地址
-----------------
*/

//创建缓冲区对应的free或busy空间，Rb为free或busy指针，Type为缓冲区中数据的地址类型，Size为缓冲区大小
#define CREATE_RING_BUFFRE(Rb, Type, Size)    (Rb) = malloc(sizeof(ring_buffer_t) + (sizeof(Type) * (Size)));\
                                            (Rb)->block=(Size);(Rb)->size=sizeof(Type);(Rb)->r=(Rb)->w=0

//初始化free空间，Buf为缓冲区首地址，Free为free指针，Type为缓冲区中数据类型
#define INIT_RING_BUFFER(Buf, Free, Type)   do {\
                                                if((Free)->w < (Free)->block) {\
                                                    (Free)->buf[(Free)->w]=(uint8_t *)((char *)(Buf)+((Free)->w * sizeof(Type)));\
                                                    (Free)->w++;\
                                                }else {\
                                                    break;\
                                                }\
                                            }while(1)

//判断是否可以从free或busy空间中得到缓冲区中数据的地址，FB为free或busy指针，Data为从缓冲区中得到的数据地址，Type为缓冲区中数据的地址类型
//return 1 表示可以得到，return 0 表示不能得到
#define CAN_READ(FB, Data, Type)    (((FB)->r != (FB)->w) ? ((Data=(Type)((FB)->buf[(FB)->r])) ? 1 : 0) : 0)

//从free或busy空间中得到缓冲区中数据的地址，Free为free指针，Data为从缓冲区中得到的数据地址，Type为缓冲区中数据的地址类型
//return 1 表示成功，return 0 表示失败
#define READ_ADDR_FROM_BUFFER(FB, Data, Type)     (CAN_READ(FB, Data, Type) ? (((FB)->r=(((FB)->r+1)%(FB)->block)) ? 1 : 0) : 0)

//判断是否可以将数据写到缓冲区中，FB为free或busy指针，Data为写入缓冲区中的数据
//return 1 表示可以写到缓冲区，return 0 表示不可以写入缓冲区
#define CAN_WRITE(FB, Data)    (((((FB)->w+1)%(FB)->block) != (FB)->r) ? (((FB)->buf[(FB)->w]=(uint8_t *)(Data)) ? 1 : 0) : 0)

//将数据写入到缓冲区中，FB为free或busy指针，Data为写入缓冲区中的数据
//return 1 表示成功，return 0 表示失败
#define WRITE_ADDR_TO_BUFFER(FB, Data)  (CAN_WRITE(FB, Data) ? (((FB)->w=(((FB)->w+1)%(FB)->block)) ? 1 : 0) : 0)

#endif
