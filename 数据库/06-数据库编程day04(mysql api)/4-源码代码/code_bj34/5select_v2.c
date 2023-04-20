#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

    //8. 释放结果集
    //void mysql_free_result(MYSQL_RES *result)  
    mysql_free_result(res); 
    

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

    //3. 执行select语句
    memset(buf, 0, SIZE);
    strcpy(buf, "select * from dept");
    ret = mysql_query(mysql, buf);
    if(0 != ret)
    {
        printf("mysql_query %s\n", mysql_error(mysql));
        goto err1;
    }
    else
    {
        //printf("%lu rows affected....\n", mysql_affected_rows(mysql)); 
    }

    //4. 获取结果集
    res = mysql_store_result(mysql);
    if (NULL == res)
    {
        printf("mysql_store_result failed...\n"); 
        goto err1;
    }

    //5. 显示结果集
    show_result(res);

    //6. 关闭连接
    mysql_close(mysql); 

    return 0;
err1:
    mysql_close(mysql); 
err0:
    return 1;
}
