#include <iostream>
#include <stdio.h>
#include <mysql.h>
using namespace std;
#pragma comment(lib, "libmysql.lib") 
#pragma comment(lib, "ws2_32.lib")   

#define DB_HOST "127.0.0.1"//"호스트IP, 도메인또는localhost"
#define DB_USER "root"
#define DB_PASS "1234"
#define DB_NAME "test"
#define SQL_INSERT_RECORD "INSERT INTO test_table VALUES (121320, 'ggg');"

int main()
{
	MYSQL *connection = NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	int i;

	char query[255];

	mysql_init(&conn);

	// DB 연결
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}

	//insert
	query_stat = mysql_query(connection, SQL_INSERT_RECORD);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	
	//close
	mysql_close(connection);

	return 0;
}