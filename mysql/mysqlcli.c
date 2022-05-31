#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<mysql/mysql.h>

int main()
{
    MYSQL mysql_conn;
    MYSQL * mysql = mysql_init(&mysql_conn);
    if(mysql == NULL)
    {
        printf("mysql init err\n");
        exit(1);
    }

    mysql = mysql_real_connect(&mysql_conn,"localhost","root","123456","c218db",3306,NULL,0);
    if(mysql == NULL)
    {
        printf("connect err\n");
        exit(1);
    }

    printf("连接成功\n");
    
    char* sql="insert into student values(1001,'小白',23)";
    int res = mysql_query(mysql,sql);
    if(res == 0)
    {
        printf("插入成功\n");
    }

    mysql_close(mysql);

    exit(0);
}
