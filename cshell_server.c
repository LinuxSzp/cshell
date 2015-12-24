/*************************************************************************
    > File Name: cshell_server.c
    > Author: linux
    > Mail: linuxszp@icloud.com 
    > Created Time: 2015年12月18日 星期五 15时04分48秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "cshell_server.h"
#include "cshell_register.h"

void _create_ringbuffer(rb_t *rb)
{
    rb->buf = malloc(sizeof(string_cmd_t) * 65534);   
    CREATE_RING_BUFFRE(rb->qfree, string_cmd_t *, 65534);
    CREATE_RING_BUFFRE(rb->qbusy, string_cmd_t *, 65534);
    INIT_RING_BUFFER(rb->buf, rb->qfree, string_cmd_t);
}

/* 分配资源给连接的客户端 */
void _allocation_client_resources(clr_t *clr, uint32_t fd)
{
    clr->c_fd = fd;
    _create_ringbuffer(&clr->c_rb);
}

/* 释放客户端资源 */
void _release_client_resources(s_handle_t *sht)
{
    close(sht->clr.c_fd);
    free(sht->clr.c_rb.qfree);
    free(sht->clr.c_rb.qbusy);
    free(sht->clr.c_rb.buf);
    sht->c_flag = 0;
    memset(&sht->clr, 0, sizeof(clr_t));
}

/* 服务器接收外部(telnet)发送的字符串 */
void *_cshell_recv_string_from_outside(void *arg)
{
    fd_set rfdset;
    int ret = -1;
    s_handle_t *ptmp_t_handle = ((cshell_handle_t *)arg)->s_handle;
    char buf[CMD_LEN_MAX] = {'\0'};
    uint32_t rn = 0;    /* 接收的字节数 */
    string_cmd_t *scmd = NULL;
    char ctrl_c[] = {0xff, 0xf4, 0xff, 0xfd, 0x06};

    pthread_detach(pthread_self());

    while(1)
    {
        FD_ZERO(&rfdset);
        FD_SET(ptmp_t_handle->clr.c_fd, &rfdset);

        ret = select(ptmp_t_handle->clr.c_fd+1, &rfdset, NULL, NULL, NULL);
        if(FD_ISSET(ptmp_t_handle->clr.c_fd, &rfdset))
        {
            ret = read(ptmp_t_handle->clr.c_fd, buf+rn, 1);
            if(ret <= 0 || buf[rn] == 0x04          /* CTRL+D */
                    || strncmp(buf, ctrl_c, sizeof(ctrl_c)) == 0)     /* CTRL+C */
            {
                /* 向客户端缓冲区中写入"transmit",通知transmit线程退出 */
                while(!READ_ADDR_FROM_BUFFER(ptmp_t_handle->clr.c_rb.qfree, scmd, string_cmd_t *))
                {
                    usleep(100);
                }
                memcpy(scmd->strcmd, "transmit stop", strlen("transmit stop"));
                while(!WRITE_ADDR_TO_BUFFER(ptmp_t_handle->clr.c_rb.qbusy, scmd))
                {
                    usleep(100);
                }
                break;
            }
            if(buf[rn] == '\r' || rn >= CMD_LEN_MAX-1)   /* 回车 */
            {
                /* 吃掉'\n' */
                read(ptmp_t_handle->clr.c_fd, buf+rn, 1);

                /* 将接收到的命令写入客户端缓冲区 */
                while(!READ_ADDR_FROM_BUFFER(ptmp_t_handle->clr.c_rb.qfree, scmd, string_cmd_t *))
                {
                    usleep(100);
                }
                memcpy(scmd->strcmd, buf, rn);
                while(!WRITE_ADDR_TO_BUFFER(ptmp_t_handle->clr.c_rb.qbusy, scmd))
                {
                    usleep(100);
                }

                /* 清空buf,接收下一个命令 */
                memset(buf, 0, sizeof(buf));
                rn = 0;
            }
            else
            {
                rn += ret;
            }
        }
    }

    return (void *)0;
}

/* 服务器将字符串转发给注册程序 */
void *_cshell_transmit_string_to_inside(void *arg)
{
    s_handle_t *ptmp_t_handle = ((cshell_handle_t *)arg)->s_handle;
    string_cmd_t *scmd = NULL;
    string_cmd_t *cmd = NULL;

    pthread_detach(pthread_self());

    while(1)
    {
        while(!READ_ADDR_FROM_BUFFER(ptmp_t_handle->clr.c_rb.qbusy, scmd, string_cmd_t *))
        {
            usleep(100);
        }
        if((strncmp(scmd->strcmd, "transmit stop", strlen("transmit stop")) == 0))
        {
            /* 客户端已断开连接 */
            _release_client_resources(ptmp_t_handle);
            break;
        }

        /* 将命令放在服务器缓冲区,供内部(inside)访问 */
        while(!READ_ADDR_FROM_BUFFER(ptmp_t_handle->s_rb.qfree, cmd, string_cmd_t *))
        {
            usleep(100);
        }
        memcpy(cmd->strcmd, scmd->strcmd, strlen(scmd->strcmd));
        while(!WRITE_ADDR_TO_BUFFER(ptmp_t_handle->s_rb.qbusy, cmd))
        {
            usleep(100);
        }

        while(!WRITE_ADDR_TO_BUFFER(ptmp_t_handle->clr.c_rb.qfree, scmd))
        { 
            usleep(100);
        }
    }

    return (void *)0;
}

LONG cshell_create_server_handle(s_handle_t **ps_handle)
{
    /* 创建服务器句柄 */
    *ps_handle = (s_handle_t *)malloc(sizeof(s_handle_t));
    if(NULL == *ps_handle)
    {
        return SERVER_W_GET_HANDLE_FAILE;
    }
    memset((*ps_handle), 0, sizeof(s_handle_t));
    (*ps_handle)->c_flag = 0;
    _create_ringbuffer(&(*ps_handle)->s_rb);

    return SERVER_S_SUCCESS;
}

LONG cshell_server_install(void *handle_arg, uint32_t iport)
{
    cshell_handle_t *handle = (cshell_handle_t *)handle_arg;
    pthread_t pid_r, pid_w;
    struct sockaddr_in saddr;
    int iconnfd = -1;
    int isfd = -1;
    int on = 1;     /* 设置套接字选项,避免绑定错误 */

    signal(SIGPIPE, SIG_IGN);

    saddr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &(saddr.sin_addr));
    saddr.sin_port = htons(iport);

    if((isfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        return SERVER_W_INSTALL_FAILED;
    }

    if(setsockopt(isfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
    {
        close(isfd);
        return SERVER_W_INSTALL_FAILED;
    }

    if(bind(isfd, (struct sockaddr *)&saddr, sizeof(struct sockaddr)) < 0)
    {
        close(isfd);
        return SERVER_W_INSTALL_FAILED;
    }

    listen(isfd, 5);

    while(1)
    {
        iconnfd = accept(isfd, NULL, NULL);
        if(iconnfd)
        {
            /* 同时只能有一个telnet连接 */
            if(handle->s_handle->c_flag == 0)
            {
                handle->s_handle->c_flag = 1;
                _allocation_client_resources(&handle->s_handle->clr, iconnfd);

                write(iconnfd, "\nEnter CTRL+D or CTRL+C for exit.", 
                        strlen("\nEnter CTRL+D or CTRL+C for exit."));
                write(iconnfd, "\n>", strlen("\n>"));

                pthread_create(&pid_r, NULL, _cshell_recv_string_from_outside, handle);
                pthread_create(&pid_w, NULL, _cshell_transmit_string_to_inside, handle);
            }
            else
            {
                /* 上个telnet连接未断开,关闭此次的telnet连接 */
                close(iconnfd);
            }
        }
    }

    return SERVER_S_INSTALL_SUCCESS;
}
