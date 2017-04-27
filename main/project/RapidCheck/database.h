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
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
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

private:
	void insert(char * query) 
	{
		int query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			return;
		}
	}
	void select(char * query, vector<vector<int> >& rows)
	{

		int query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			return;
		}
		sql_result = mysql_store_result(connection);
		int num_fileds = mysql_num_fields(sql_result);

		// read row
		while (sql_row = mysql_fetch_row(sql_result))
		{
			vector<int> row;
			for (int i = 0; i < num_fileds; i++)
			{
				int val = sql_row[i] ? atoi(sql_row[i]) : 0;
				row.push_back(val);
			}
			rows.push_back(row);
		}
	}
public:
	void insertTracking(int videoId, int objectId, int frameNum, int x, int y, int width, int height) 
	{
		char query[200];
		sprintf(query, "INSERT INTO tracking VALUES (%d,%d,%d,%d,%d,%d,%d);", videoId, objectId, frameNum, x, y, width, height);
		insert(query);
	}
	void insertDetection(int videoId, int frameNum, int classId, int x, int y, int width, int height) 
	{
		char query[200];
		sprintf(query, "INSERT INTO detection (videoId, frameNum, classId, x, y, width, height) VALUES (%d,%d,%d,%d,%d,%d,%d);", videoId, frameNum, classId, x, y, width, height);
		insert(query);
	}
	
	void selectTracking(vector<vector<int> >& rows, int videoId, int start_frame, int end_frame, int frame_step)
	{
		char query[200];
		sprintf(query, "SELECT * FROM tracking WHERE videoId = %d AND frameNum >= %d AND frameNum < %d AND frameNum %c %d = %d;", videoId, start_frame, end_frame, '%', frame_step, start_frame%frame_step);
		select(query, rows);
	}
	void selectDetection(vector<vector<int> >& rows, int videoId, int start_frame, int end_frame, int frame_step)
	{
		char query[200];
		sprintf(query, "SELECT * FROM detection WHERE videoId = %d AND frameNum >= %d AND frameNum < %d AND frameNum %c %d = %d;", videoId, start_frame, end_frame, '%', frame_step, start_frame%frame_step);
		select(query, rows);
	}
	~DB() {
		mysql_close(connection);
	}
};