/*************************************************************************
    > File Name: cshell_error.h
    > Author: linux
    > Mail: linuxszp@icloud.com 
    > Created Time: 2015年12月18日 星期五 15时40分17秒
 ************************************************************************/

#ifndef _CSHELL_ERROR_H_
#define _CSHELL_ERROR_H_

#ifndef LONG
    typedef long    LONG;
#endif

#define CSHELL_S_SUCCESS            ((LONG)0x00000000)  /**< 没有遇到任何错误 */
#define CSHELL_W_INIT_FAILED        ((LONG)0x80100001)  /**< 初始化失败 */
#define SERVER_S_SUCCESS            CSHELL_S_SUCCESS    /**< 服务器没有遇到任何错误 */
#define SERVER_W_GET_HANDLE_FAILE   ((LONG)0x80100002)  /**< 服务器获得句柄失败 */
#define SERVER_S_INSTALL_SUCCESS    SERVER_S_SUCCESS    /**< 服务器部署没有遇到错误 */
#define SERVER_W_INSTALL_FAILED     ((LONG)0x80100003)  /**< 服务器部署出现故障 */
#define LIST_S_SUCCESS              SERVER_S_SUCCESS    /**< 链表操作没有遇到错误 */
#define LIST_W_GET_HANDLE_FAILE     ((LONG)0x80100004)  /**< 链表获得句柄失败 */
#define LIST_W_CMD_INSERT_FAILED    ((LONG)0x80100005)  /**< 命令链表插入失败 */
#define LIST_W_CMD_NO_FIND          ((LONG)0x80100006)  /**< 命令未找到 */

#endif
