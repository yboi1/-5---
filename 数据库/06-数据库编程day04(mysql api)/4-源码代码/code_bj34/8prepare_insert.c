#include <stdio.h>
#include "mysql.h"
#include <stdlib.h>
#include <string.h>

#define _HOST_ "192.168.11.42"  //主机
#define _USER_ "root"       //mysql用户,非主机
#define _PASSWD_ "123456"   //密码
#define _DBNAME_ "scott"    //库名

#define STRING_SIZE 50

#define DROP_SAMPLE_TABLE "DROP TABLE IF EXISTS test_table"
#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table(col1 INT,\
    col2 VARCHAR(40),\
col3 SMALLINT,\
col4 TIMESTAMP)"
#define INSERT_SAMPLE "INSERT INTO test_table(col1,col2,col3) VALUES(?, ?, ?)"

void prepare_insert(MYSQL *mysql);

int main()
{
    //1.初始化
    MYSQL * mysql = NULL;
    mysql = mysql_init(NULL) ;
    if(mysql == NULL )
    {
        printf("mysql init err\n");
        exit(1);
    }
    //2.连接
    mysql = mysql_real_connect(mysql, _HOST_,_USER_, _PASSWD_,_DBNAME_, 0, NULL,0);
    if(mysql == NULL)
    {
        printf("mysql_real_connect connect err\n");
        exit(1);
    }
    printf("welcome to mysql \n");

    prepare_insert(mysql);

    //3.关闭
    mysql_close(mysql);
    return 0;
}




void prepare_insert(MYSQL *mysql)
{
    MYSQL_STMT    *stmt;//预处理的句柄
    MYSQL_BIND    bind[3];//绑定变量 
    my_ulonglong  affected_rows;
    int           param_count;
    short         small_data;
    int           int_data;
    char          str_data[STRING_SIZE];
    unsigned long str_length;
    my_bool       is_null;

    if (mysql_query(mysql, DROP_SAMPLE_TABLE))//删除表
    {
        fprintf(stderr, " DROP TABLE failed\n");
        fprintf(stderr, " %s\n", mysql_error(mysql));
        exit(0);
    }

    if (mysql_query(mysql, CREATE_SAMPLE_TABLE))//创建表
    {
        fprintf(stderr, " CREATE TABLE failed\n");
        fprintf(stderr, " %s\n", mysql_error(mysql));
        exit(0);
    }

    /* Prepare an INSERT query with 3 parameters */
    /* (the TIMESTAMP column is not named; the server */
    /*  sets it to the current date and time) */
    stmt = mysql_stmt_init(mysql); //预处理的初始化
    if (!stmt)
    {
        fprintf(stderr, " mysql_stmt_init(), out of memory\n");
        exit(0);
    }
    if (mysql_stmt_prepare(stmt, INSERT_SAMPLE, strlen(INSERT_SAMPLE))) //insert 语句 的预处理 
    {
        fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
    }
    fprintf(stdout, " prepare, INSERT successful\n");

    /* Get the parameter count from the statement */
    param_count= mysql_stmt_param_count(stmt);//获得参数个数 
    fprintf(stdout, " total parameters in INSERT: %d\n", param_count);

    if (param_count != 3) /* validate parameter count */
    {
        fprintf(stderr, " invalid parameter count returned by MySQL\n");
        exit(0);
    }

    /* Bind the data for all 3 parameters */

    memset(bind, 0, sizeof(bind));

    /* INTEGER PARAM */
    /* This is a number type, so there is no need to specify buffer_length */
    bind[0].buffer_type= MYSQL_TYPE_LONG;
    bind[0].buffer= (char *)&int_data;//内存地址的映射 
    bind[0].is_null= 0;
    bind[0].length= 0;

    /* STRING PARAM */
    bind[1].buffer_type= MYSQL_TYPE_STRING;
    bind[1].buffer= (char *)str_data;//char 100 
    bind[1].buffer_length= STRING_SIZE;
    bind[1].is_null= 0;
    bind[1].length= &str_length;

    /* SMALLINT PARAM */
    bind[2].buffer_type= MYSQL_TYPE_SHORT;
    bind[2].buffer= (char *)&small_data;
    bind[2].is_null= &is_null;//是否为null的指示器 
    bind[2].length= 0;

    /* Bind the buffers */
    if (mysql_stmt_bind_param(stmt, bind)) //绑定变量 参数绑定
    {
        fprintf(stderr, " mysql_stmt_bind_param() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
    }

    //第一波赋值 
    int_data= 10;             /* integer */
    strncpy(str_data, "MySQL", STRING_SIZE); /* string  */
    str_length= strlen(str_data);

    /* INSERT SMALLINT data as NULL */
    is_null= 1;//指示插入的第三个字段是否为null 

    /* Execute the INSERT statement - 1*/
    if (mysql_stmt_execute(stmt)) //预处理的执行,第一次执行 
    {
        fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
    }

    /* Get the total number of affected rows */
    affected_rows= mysql_stmt_affected_rows(stmt);//预处理的影响条数
    fprintf(stdout, " total affected rows(insert 1): %lu\n",
            (unsigned long) affected_rows);

    if (affected_rows != 1) /* validate affected rows */
    {
        fprintf(stderr, " invalid affected rows by MySQL\n");
        exit(0);
    }

    //第二波赋值 
    int_data= 1000;
    strncpy(str_data, "The most popular Open Source database", STRING_SIZE);
    str_length= strlen(str_data);
    small_data= 1000;         /* smallint */
    is_null= 0;               /* reset */

    /* Execute the INSERT statement - 2*/
    if (mysql_stmt_execute(stmt))//第二次执行
    {
        fprintf(stderr, " mysql_stmt_execute, 2 failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
    }

    /* Get the total rows affected */
    affected_rows= mysql_stmt_affected_rows(stmt);
    fprintf(stdout, " total affected rows(insert 2): %lu\n",
            (unsigned long) affected_rows);

    if (affected_rows != 1) /* validate affected rows */
    {
        fprintf(stderr, " invalid affected rows by MySQL\n");
        exit(0);
    }

    /* Close the statement */
    if (mysql_stmt_close(stmt))
    {
        fprintf(stderr, " failed while closing the statement\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
    }

}
