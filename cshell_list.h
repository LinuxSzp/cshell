/*************************************************************************
    > File Name: cshell_llist.h
    > Author: linux
    > Mail: linuxszp@icloud.com 
    > Created Time: 2015年12月21日 星期一 10时27分02秒
 ************************************************************************/

#ifndef _CSHELL_LIST_H_
#define _CSHELL_LIST_H_

#include "cshell_error.h"

#define CMD_LEN_MAX 64
#define DESCRIPTION_LEN_MAX 128

typedef struct _cmd_t_
{
    char cmd[CMD_LEN_MAX];           /* 命令 */
    char desc[DESCRIPTION_LEN_MAX];     /* 命令描述 */
    void (*fun)(void *);                /* 执行函数 */
    void *arg;                      /* 执行函数参数 */
} cmd_t;

typedef struct _cmd_list_node_t_
{
    cmd_t        st_cmd;
    struct _cmd_list_node_t_ *next;
} cmd_list_node_t;

typedef struct _cmd_header_t_
{
    cmd_list_node_t *cmd_node_1st;
} cmd_header_t;

/* 创建一个命令链表头 */
LONG cshell_create_cmd_list_header(/*OUT*/ cmd_header_t **cmd_header);

/* 销毁整个命令链表 */
void cshell_destroy_cmd_list(cmd_header_t *cmd_handle);

/* 向命令链表中插入一条注册的命令 */
LONG cshell_insert_cmd_to_list(cmd_header_t *cmd_header, char *cmd, char *desc, void (*fun)(void *), void *arg);

/* 查找命令,浅拷贝*/
LONG cshell_find_cmd_at_list(cmd_header_t *cmd_header, char *cmd, /*OUT*/ cmd_t **st_cmd);

/* 打印所有注册的命令 */
void cshell_printf_all_cmd_from_list(cmd_header_t *cmd_handle);

#endif
