#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <mysql/mysql.h>

#define __HOST__ "192.168.21.28"
#define __USER__ "root"
#define __PASSWD__ "123456"
#define __DB__ "scott"

#define SIZE 128

//显示结果集
int show_result(MYSQL_RES *res)
{

    int i, j;

    unsigned long row_nums = 0;
    unsigned int col_nums = 0;

    MYSQL_ROW row = NULL;

    MYSQL_FIELD *field = NULL;

    //1. 获取结果集中行数
    //my_ulonglong mysql_num_rows(MYSQL_RES *result) 
    row_nums = mysql_num_rows(res);
    printf("结果集中行数: %lu\n", row_nums);

    //2. 获取结果集中列数
    //unsigned int mysql_num_fields(MYSQL_RES *result)
    col_nums = mysql_num_fields(res);
    printf("结果集中列数: %u\n", col_nums);

    //3. 获取所有字段信息
    field = mysql_fetch_fields(res);

    printf("+----------------+----------------+----------------+\n|");
    //输出字段信息
    for (i = 0; i < col_nums; i++)
    {
        printf("%15s |", field[i].name); 
    }

    printf("\n+----------------+----------------+----------------+\n");

    //7. 输出所有的数据
    for (i = 0; i < row_nums; i++)    
    {
        putchar('|');
        //获取一行
        row = mysql_fetch_row(res); 
        if (NULL == row)
        {
            break; 
        }

        //循环输出每一列值
        for (j = 0; j < col_nums; j++)     
        {
            printf("%15s |", row[j] ? row[j] : "NULL"); 
        }

        //换行
        printf("\n");
    }
    printf("+----------------+----------------+----------------+\n");
    printf("%lu rows in set (0.00 sec)\n\n", row_nums);

    //8. 释放结果集
    //void mysql_free_result(MYSQL_RES *result)  
    mysql_free_result(res); 

    return 0;
}


//显示MYSQL信息
int show_board(void)
{
    //清屏
    system("clear");

    printf("Welcome to the MySQL monitor.  Commands end with ; or .\\g\n");
    printf("Your MySQL connection id is 77\n");
    printf("Server version: 5.7.23 MySQL Community Server (GPL)\n");
    printf("Copyright (c) 2000, 2018, Oracle and/or its affiliates. All rights reserved\n");
    printf("Oracle is a registered trademark of Oracle Corporation and/or its\n");
    printf("Type 'help;' or '\\h' for help. Type '\\c' to clear the current input statement\n");

    printf("\n");
    return 0;
}

int main(void)
{

    int ret = -1;
    MYSQL *mysql = NULL;
    MYSQL_RES *res = NULL;

    char buf[SIZE];

    //1. 初始化
    mysql = mysql_init(NULL);
    if (NULL == mysql)
    {
        printf("mysql_init failed..\n"); 
        goto err0;
    }
    printf("mysql_init ok...\n");

    //2. 连接到MySQL数据库
    mysql = mysql_real_connect(mysql, __HOST__, __USER__, __PASSWD__, __DB__, 0, NULL, 0);
    if (NULL == mysql)
    {
        printf("mysql_real_connect failed...\n");
        goto err1;
    }
    printf("连接到MySQL数据库ok....\n");

    //显示MySQL信息
    show_board();

    while(1)
    {
        //输出mysql>提示信息
        write(STDOUT_FILENO, "mysql> ", strlen("mysql> ")); 
        //等待用户输入
        memset(buf, 0, SIZE);
        read(STDIN_FILENO, buf, SIZE);

        //将换行符替换成\0
        buf[strlen(buf) - 1] = '\0';

        //退出客户端
        if ((strncmp(buf, "quit", 4) == 0) || (strncmp(buf, "exit", 4) == 0))
        {
            printf("Bye\n");  
            break;
        }

        //如果输入的回车 直接继续
        if (buf[0] == '\0')
            continue;

        //printf("buf:%s\n", buf);
        //执行sql语句 
        ret = mysql_query(mysql, buf);
        if(0 != ret)
        {
            printf("mysql_query %s\n", mysql_error(mysql));
        }

        //4. 获取结果集
        res = mysql_store_result(mysql);
        if (NULL == res)
        {
            printf("Query OK, %lu row affected (0.00 sec)\n\n", mysql_affected_rows(mysql));
        }
        else
        {
            //显示结果集
            show_result(res);
        }
    }


    //6. 关闭连接
    mysql_close(mysql); 

    return 0;
err1:
    mysql_close(mysql); 
err0:
    return 1;
}
