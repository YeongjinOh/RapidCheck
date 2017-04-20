#include <mysql.h>
#include <stdio.h>
#include <iostream>

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "1234"
#define DB_NAME "rapidcheck"

#pragma comment(lib, "libmysql.lib") 
#pragma comment(lib, "ws2_32.lib")   

class DB {
	MYSQL *connection = NULL, conn;
	//MYSQL_RES *sql_result;
	//MYSQL_ROW sql_row;
public:
	DB() {
		mysql_init(&conn);
		connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
		if (connection == NULL)
		{
			fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
			return;
		}
	}

	void insert(char * query) {
		int query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			return;
		}
	}
	void insert(int fileId, int objectId, int frameNum, int x, int y, int width, int height) {
		char query[200];
		sprintf(query, "INSERT INTO tracking VALUES (%d,%d,%d,%d,%d,%d,%d);", fileId, objectId, frameNum, x, y, width, height);
		int query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			return;
		}
	}
	~DB() {
		mysql_close(connection);
	}
};