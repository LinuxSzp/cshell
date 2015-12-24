/*************************************************************************
    > File Name: cshell_demo.c
    > Author: linux
    > Mail: linuxszp@icloud.com 
    > Created Time: 2015年12月21日 星期一 15时19分45秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#include "cshell_register.h"

typedef struct _ip_packet_t_
{
    uint64_t    total;              //包总数
    uint64_t    capture_total;      //抓包
    uint64_t    loss_total;         //丢包
} ip_pkt_t;

uint64_t second = 0;

void *timing_thread(void *arg)
{
    while(1)
    {
        sleep(1);
        if(*(int *)arg == 0)
        {
            second++;
        }
    }

    return arg;
}

void *cshell_thread(void *arg)
{
    cshell_init(1234);

    return arg;
}

void show_pkt_total(void *arg)
{
    ip_pkt_t *ip_pkt = (ip_pkt_t *)arg;

    printf("-------------------------------------------------------------------------\n");
    printf("|   cap_pkt   |   loss_pkt  |    total    |  cap_v(n/s) | loss_v(n/s) |\n");
    printf("-------------------------------------------------------------------------\n");
    printf("|%13"PRIu64"|%13"PRIu64"|%13"PRIu64"|%13"PRIu64"|%13"PRIu64"|\n", \
            ip_pkt->capture_total, ip_pkt->loss_total, ip_pkt->total, \
            ip_pkt->capture_total/second, ip_pkt->loss_total/second);
    printf("-------------------------------------------------------------------------\n");
}

void clear_pkt_total(void *arg)
{
    ip_pkt_t *ip_pkt = (ip_pkt_t *)arg;

    ip_pkt->total = 0;
    ip_pkt->loss_total = 0;
    ip_pkt->capture_total = 0;   
}

void start_crawl_pkt(void *arg)
{
    *((int *)arg) = 0;
    printf("start catch\n");
}

void stop_crawl_pkt(void *arg)
{
    *((int *)arg) = 1;
    printf("stop catch\n");
}

int main()
{
    pthread_t pid;
    pthread_t pid_ts;
    uint64_t unum = 0;
    ip_pkt_t ip_pkt = {0};
    int stop = 0;

    pthread_create(&pid, NULL, cshell_thread, NULL);
    pthread_create(&pid_ts, NULL, timing_thread, &stop);

    cshell_cmd_register("start", "start crawl packet", start_crawl_pkt, &stop);
    cshell_cmd_register("stop", "stop crawl packet", stop_crawl_pkt, &stop);
    cshell_cmd_register("watch", "show capture and loss packet total", show_pkt_total, &ip_pkt);
    cshell_cmd_register("clear", NULL, clear_pkt_total, &ip_pkt);
    
    while(1)
    {
        if(stop == 0)
        {
            ip_pkt.total++;
            unum++;
            if(unum % 2 == 0)
            {
                ip_pkt.capture_total++;
            }
            if(unum % 6 == 0)
            {
                ip_pkt.loss_total++;
            }
            ip_pkt.total = ip_pkt.capture_total + ip_pkt.loss_total;
        }
        usleep(200);
    }

    return 0;
}
