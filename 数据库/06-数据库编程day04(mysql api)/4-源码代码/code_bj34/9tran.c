//mysql�е�����
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql.h"

#define SET_TRAN	"SET AUTOCOMMIT=0"  		//�ֶ�commit	���������ֶ�commit
#define UNSET_TRAN	"SET AUTOCOMMIT=1"			//�Զ�commit

#define _HOST_ "127.0.0.1"
#define _USER_ "root"
#define _PASSWD_ "123"
#define _DBNAME_ "scott"

//��������Ϊ�ֶ��ύ
int mysql_OperationTran(MYSQL *mysql)  			
{
    //--��������
    int ret = mysql_query(mysql, "start transaction");  
    if (ret != 0) {
        printf("mysql_OperationTran query start err: %s\n", mysql_error(mysql));
        return ret;
    }

    //--��������Ϊ�ֶ��ύ
    ret = mysql_query(mysql, SET_TRAN);			//set��autocommmit = 0
    if (ret != 0) {
        printf("mysql_OperationTran query set err: %s\n", mysql_error(mysql));
        return ret;
    }

    return ret;
}

//��������Ϊ�Զ��ύ
int mysql_AutoTran(MYSQL *mysql)
{
    //--��������
    int ret = mysql_query(mysql, "start transaction");  
    if (ret != 0) {
        printf("mysql_AutoTran query start err: %s\n", mysql_error(mysql));
        return ret;
    }

    //--��������Ϊ�Զ��ύ
    ret = mysql_query(mysql, UNSET_TRAN);  //"set autocommit = 1"
    if (ret != 0) {
        printf("mysql_AutoTran query set err: %s\n", mysql_error(mysql));
        return ret;
    }

    return ret;		
}

//ִ��commit���ֶ��ύ����
int mysql_Commit(MYSQL *mysql)
{
    int ret = mysql_query(mysql, "COMMIT"); //�ύ
    if (ret != 0) {
        printf("commit err: %s\n", mysql_error(mysql));
        return ret;
    }
    return ret;
}

//ִ��rollback���ع�����		
int mysql_Rollback(MYSQL *mysql)
{
    int ret = mysql_query(mysql, "ROLLBACK");
    if (ret != 0) {
        printf("rollback err: %s\n", mysql_error(mysql));
        return ret;
    }
    return ret;

}

#define DROP_SAMPLE_TABLE "DROP TABLE IF EXISTS test_table"
#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table(col1 INT,\
    col2 VARCHAR(10),\
col3 VARCHAR(10))"

#define sql01 "INSERT INTO test_table(col1,col2,col3) VALUES(10, 'AAA', 'A1')"
#define sql02 "INSERT INTO test_table(col1,col2,col3) VALUES(20, 'BBB', 'B2')"
#define sql03 "INSERT INTO test_table(col1,col2,col3) VALUES(30, 'CCC', 'C3')"
#define sql04 "INSERT INTO test_table(col1,col2,col3) VALUES(40, 'DDD', 'D4')"

int main(void)
{
    int ret = 0;

    //1. ��ʼ��һ�����
    MYSQL *mysql = mysql_init(NULL);

    //2. ����MySQL���ݿ�
    mysql = mysql_real_connect(mysql, _HOST_, _USER_, _PASSWD_, _DBNAME_, 0, NULL, 0);
    if (mysql == NULL) {
        ret = mysql_errno(mysql);
        printf("func mysql_real_connect() err��%d\n", ret);
        return ret;
    } 	
    printf(" --- connect ok......\n");	

    //ִ��ɾ����
    if (mysql_query(mysql, DROP_SAMPLE_TABLE)) {
        fprintf(stderr, " DROP TABLE failed\n");
        fprintf(stderr, " %s\n", mysql_error(mysql));
        exit(0);
    }
    //ִ�д�����
    if (mysql_query(mysql, CREATE_SAMPLE_TABLE)) {
        fprintf(stderr, " CREATE TABLE failed\n");
        fprintf(stderr, " %s\n", mysql_error(mysql));
        exit(0);
    }	

    //��������
    ret = mysql_OperationTran(mysql); 	//�������񣬲��޸���������Ϊ�ֶ�commit 
    if (ret != 0) {
        printf("mysql_OperationTran() err:%d\n", ret);
        return ret;
    }

    ret = mysql_query(mysql, sql01);	//����в����һ������ ��AAA��
    if (ret != 0) {
        printf("mysql_query() err:%d\n", ret);
        return ret;
    }

    ret = mysql_query(mysql, sql02);	//����в���ڶ������� ��BBB��
    if (ret != 0) {
        printf("mysql_query() err:%d\n", ret);
        return ret;
    }

    ret = mysql_Commit(mysql); 			//�ֶ��ύ����
    if (ret != 0) {
        printf("mysql_Commit() err:%d\n", ret);
        return ret;
    }
    //////////AAA BBB  ��ȥ�ˡ�

#if 1
    ret = mysql_AutoTran(mysql); 		// =�ٴ�= �޸���������Ϊ���Զ���commit
    if (ret != 0) {
        printf("mysql_OperationTran() err:%d\n", ret);
        return ret;
    }
#else 
    ret = mysql_OperationTran(mysql); 	// =�ٴ�= �޸���������Ϊ���ֶ���commit
    if (ret != 0) {
        printf("mysql_OperationTran() err:%d\n", ret);
        return ret;
    }
#endif

    ret = mysql_query(mysql, sql03);	//����в������������ ��CCC��
    if (ret != 0) {
        printf("mysql_query() err:%d\n", ret);
        return ret;
    }

    ret = mysql_query(mysql, sql04);	//����в������������ ��DDD��
    if (ret != 0) {
        printf("mysql_query() err:%d\n", ret);
        return ret;
    }

    ret = mysql_Rollback(mysql);		//ֱ��rollback����
    if (ret != 0) {
        printf("mysql_Rollback() err:%d\n", ret);
        return ret;
    }

    //rollback�����Ƿ��ܻ��˵�CCC��DDD��ֵ��ȡ�����������ԡ�

    mysql_close(mysql);

    return 0;	
}




