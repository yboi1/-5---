#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <mysql/mysql.h>

//编译方法
//[deng@localhost share]$ gcc 1mysql_init.c -L/usr/lib64/mysql/ -lmysqlclient

int main(void)
{
    MYSQL *mysql = NULL;

    //1. 初始化
    mysql = mysql_init(NULL);
    if (NULL == mysql)
    {
        printf("mysql_init failed...\n"); 
        return 1;
    }
    printf("mysql初始化Ok....\n");

    //2. 连接到MySQL数据库
    mysql = mysql_real_connect(mysql, "192.168.21.28", "root", "123456", "scott", 0, NULL, 0);
    if (NULL == mysql)
    {
        printf("mysql_real_connect failed...\n"); 
    }
    else
    {
        printf("mysql_real_connect to MySQL ok....\n"); 
    }

    //3. 断开连接
    mysql_close(mysql);

    printf("hello world\n");
    return 0;
}
