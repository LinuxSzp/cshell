/*************************************************************************
    > File Name: cshell_server.h
    > Author: linux
    > Mail: linuxszp@icloud.com 
    > Created Time: 2015年12月18日 星期五 17时20分22秒
 ************************************************************************/

#ifndef _CSHELL_SERVER_H_
#define _CSHELL_SERVER_H_

#include "rb.h"
#include "cshell_list.h"

typedef struct _string_cmd_t_
{
    char strcmd[CMD_LEN_MAX];
} string_cmd_t;

typedef struct __ring_buffer_t
{
    void    *buf;
    ring_buffer_t   *qfree;
    ring_buffer_t   *qbusy;
} rb_t;

typedef struct _client_link_resource_t
{
    uint32_t    c_fd;
    rb_t        c_rb;
} clr_t;

typedef struct _server_handle_t
{
    uint32_t        c_flag; /* 0:无连接 1:有连接*/
    clr_t           clr;   /* 客户端连接分配的资源 */
    rb_t            s_rb;
} s_handle_t;

/* 创建服务器句柄,并初始化 */
LONG cshell_create_server_handle(/*OUT*/ s_handle_t **ps_handle);

LONG cshell_server_install(/* cshell_handle_t */ void *handle, uint32_t port);

#endif
