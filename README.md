# cshell
    cshell主要用于完成对软件的调试,监控,数据分析等。

# 背景
    提高项目开发效率,对项目中各模块的单独测试,提高项目整合效率
和组成员间的配合；方便对软件中的一些数据进行实时监控,做到预防、
干预和分析处理。

# 方案
    在服务器和客户端之间建立转发机制
    过程:
    [1].通过telnet连接服务器;
    [2].服务器通过接收模块,接收telnet传来的数据,直到收到回车,这
        一接收过程结束;
    [3].服务器调用命令模块对收到的字符串进行匹配,如果没匹配到,
        给telnet发送已注册的命令列表;
    [4].服务器匹配命令成功,转发给软件,调用相应的处理函数;
    [5].软件将处理函数中的打印信息通过服务器转发给telnet显示.

# 使用方法
    * 在程序端,调用libcshell.a库中的注册函数,注册命令;

    * 在另一个终端使用telnet连接,可以
        * ls ,              显示已经注册的命令;
        * 输入注册的命令 ,  显示相应的结果信息

# 使用示例
    伪代码
    void test_fun()
    {
        printf("----------\n");
        printf("|--test--|\n");
        printf("----------\n");
    }
    void *cshell_thread(void *arg)
    {
        cshell_init();
        return NULL;
    }
    int main(int argc, char *argv[])
    {
        ....
        pthread_create(&tid, NULL, cshell_thread, NULL);
        ....

        ....
        cshell_cmd("test", test_fun);
        ....

        return 0;
    }
    编译,运行

    另一个终端,运行telnet
    # telnet 127.0.0.1 6666
    ...
    ls
        $cmd: 1. test
    test
    ----------
    |--test--|
    ----------

# 联系方式
    邮箱: linuxszp@gmail.com

# License
    MIT license
