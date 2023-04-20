#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <mysql/mysql.h>

#define __HOST__ "192.168.21.28"
#define __USER__ "root"
#define __PASSWD__ "123456"
#define __DB__ "scott"

#define SIZE 128

int main(void)
{
    int ret = -1;

    MYSQL *mysql = NULL;

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

    //3. 执行SQL语句
    memset(buf, 0, SIZE);
    //执行delete语句
    //strcpy(buf, "delete from dept where deptno = 1");
    //strcpy(buf, "delete from dept where deptno >= 99");
    //执行update语句
    //update dept set dname = '50name', loc = '50loc' where deptno = 50
    strcpy(buf, "update dept set dname = '50name', loc = '50loc' where deptno = 50");
    ret = mysql_query(mysql, buf);
    if(0 != ret)
    {
        printf("mysql_query failed....\n"); 
        printf("mysql_query %s\n", mysql_error(mysql));
        goto err1;
    }
    else
    {
        printf("%lu rows affected....\n", mysql_affected_rows(mysql)); 
    }

    //4. 关闭连接
    mysql_close(mysql); 

    return 0;
err1:
    mysql_close(mysql); 
err0:
    return 1;
}
