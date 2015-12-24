/*************************************************************************
    > File Name: cshell_register.h
    > Author: linux
    > Mail: linuxszp@icloud.com 
    > Created Time: 2015年12月23日 星期三 10时01分16秒
 ************************************************************************/

#ifndef _CSHELL_REGISTER_H_
#define _CSHELL_REGISTER_H_

#include "cshell_server.h"
#include "cshell_list.h"
#include "cshell_error.h"

typedef struct _cshell_handle_t_
{
    s_handle_t *s_handle;
    cmd_header_t *cmd_header;
} cshell_handle_t;

/*
 *  @port:绑定的端口号
*/
LONG cshell_init(uint32_t port);

/*
 *  注册命令
 * @cmd:注册的命令
 * @desc:命令描述
 * @fun:命令执行函数
 * @arg:函数fun的参数
*/
void cshell_cmd_register(char *cmd, char *desc, void (*fun)(void *), void *arg);

#endif
