/*************************************************************************
    > File Name: cshell_llist.c
    > Author: linux
    > Mail: linuxszp@icloud.com 
    > Created Time: 2015年12月21日 星期一 10时26分47秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "cshell_error.h"
#include "cshell_list.h"

LONG cshell_create_cmd_list_header(cmd_header_t **cmd_header)
{
    *cmd_header = (cmd_header_t *)malloc(sizeof(cmd_header_t));
    if(NULL == *cmd_header)
    {
        return LIST_W_CMD_INSERT_FAILED;
    }
    memset(*cmd_header, 0, sizeof(cmd_header_t));
    
    return LIST_S_SUCCESS;
}

void cshell_destroy_cmd_list(cmd_header_t *cmd_handle)
{
    cmd_list_node_t *cmd_node = NULL;
    cmd_list_node_t *cmd_next_node = NULL;
    if(NULL == cmd_handle)
    {
        return ;
    }

    cmd_node = cmd_handle->cmd_node_1st;
    while(cmd_node != NULL)
    {
        cmd_next_node = cmd_node->next;
        free(cmd_node);
        cmd_node = cmd_next_node;
    }

    cmd_handle->cmd_node_1st = NULL;
}

cmd_list_node_t *_cshell_create_cmd_list_node(char *cmd, char *desc, void (*fun)(void *), void *arg)
{
    cmd_list_node_t *cmd_node = NULL;

    if(NULL == cmd)
    {
        return NULL;
    }

    cmd_node = (cmd_list_node_t *)malloc(sizeof(cmd_list_node_t));
    if(NULL == cmd_node)
    {
        return NULL;
    }
    memset(cmd_node, 0, sizeof(cmd_list_node_t));

    memcpy(cmd_node->st_cmd.cmd, cmd, strlen(cmd));
    if(desc != NULL)
    {
        memcpy(cmd_node->st_cmd.desc, desc, strlen(desc));
    }
    cmd_node->st_cmd.fun = fun;
    cmd_node->st_cmd.arg = arg;

    return cmd_node;
}

LONG cshell_insert_cmd_to_list(cmd_header_t *cmd_header, char *cmd, char *desc, void (*fun)(void *), void *arg)
{
    cmd_list_node_t *tmp_cmd_node = NULL;
    cmd_list_node_t *cmd_node = NULL;

    cmd_node = _cshell_create_cmd_list_node(cmd, desc, fun, arg);
    if(NULL == cmd_header || NULL == cmd_node)
    {
        return LIST_W_CMD_INSERT_FAILED;
    }

    tmp_cmd_node = cmd_header->cmd_node_1st;
    if(NULL == tmp_cmd_node)
    {
        cmd_header->cmd_node_1st = cmd_node;
        cmd_node->next = NULL;
    }
    else
    {
        while(tmp_cmd_node->next != NULL)
        {
            tmp_cmd_node = tmp_cmd_node->next;
        }
        tmp_cmd_node->next = cmd_node;
        cmd_node->next = NULL;
    }

    return LIST_S_SUCCESS;
}

LONG cshell_find_cmd_at_list(cmd_header_t *cmd_header, char *cmd, cmd_t **st_cmd)
{
    cmd_list_node_t *cmd_node = NULL;

    if(NULL == cmd_header || NULL == cmd)
    {
        return LIST_W_CMD_NO_FIND;
    }

    cmd_node = cmd_header->cmd_node_1st;
    while(cmd_node != NULL)
    {
        if((strncmp(cmd_node->st_cmd.cmd, cmd, strlen(cmd_node->st_cmd.cmd)) == 0))
        {
            *st_cmd = &cmd_node->st_cmd;
            return LIST_S_SUCCESS;
        }
        cmd_node = cmd_node->next;
    }

    return LIST_W_CMD_NO_FIND;
}

void cshell_printf_all_cmd_from_list(cmd_header_t *cmd_handle)
{
    cmd_list_node_t *cmd_node = NULL;

    if(NULL == cmd_handle)
    {
        return ;
    }

    cmd_node = cmd_handle->cmd_node_1st;
    while(cmd_node != NULL)
    {
        printf("%s\n", cmd_node->st_cmd.cmd);
        cmd_node = cmd_node->next;
    }
}
