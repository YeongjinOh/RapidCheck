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

private:
	MYSQL *connection = NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;

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
	DB() {
		mysql_init(&conn);
		connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
		if (connection == NULL)
		{
			fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
			return;
		}
	}
	~DB() {
		mysql_close(connection);
	}

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
	
	void selectTracking(vector<vector<int> >& rows, int videoId, int classId, int start_frame, int end_frame, int frame_step)
	{
		char query[500];
		sprintf(query, "SELECT tracking.objectId, frameNum, x, y, width, height FROM tracking INNER JOIN objectInfo ON tracking.videoId = objectinfo.videoId and tracking.objectId = objectinfo.objectId WHERE tracking.videoId = %d AND objectinfo.classId = %d AND tracking.frameNum >= %d AND tracking.frameNum < %d AND tracking.frameNum %c %d = %d;", videoId, classId, start_frame, end_frame, '%', frame_step, start_frame%frame_step);
		select(query, rows);
	}
	void selectDetection(vector<vector<int> >& rows, int videoId, int start_frame, int end_frame, int frame_step)
	{
		char query[200];
		sprintf(query, "SELECT frameNum, x, y, width, height, classId FROM detection WHERE videoId = %d AND frameNum >= %d AND frameNum < %d AND frameNum %c %d = %d;", videoId, start_frame, end_frame, '%', frame_step, start_frame%frame_step);
		select(query, rows);
	}
	void selectDetection2(vector<vector<int> >& rows, int videoId, int classId,  int start_frame, int end_frame, int frame_step)
	{
		char query[200];
		sprintf(query, "SELECT frameNum, x, y, width, height, class FROM detection2 WHERE videoId = %d AND frameNum >= %d AND frameNum < %d AND frameNum %c %d = %d and class = %d;", videoId, start_frame, end_frame, '%', frame_step, start_frame%frame_step, classId);
		select(query, rows);
	}
	void insertObjectInfo(int videoId, int objectId, int classId, vector<float> directionRatios, double speed, vector<float> colorRatios)
	{
		if (speed >= 10000.0)
		{
			cout << "Invalid speed" << endl;
			return;
		}
		char query[1000];
		string directionKeys = "";
		for (int i = 0; i < NUM_OF_DIRECTIONS; i++)
			directionKeys += "direction" + std::to_string(i) + ", ";
		string directionValues = "";
		for (int i = 0; i < NUM_OF_DIRECTIONS; i++)
			directionValues += std::to_string(directionRatios[i]).substr(0,5) + ", ";
		string colorKeys = "";
		for (int i = 0; i < NUM_OF_COLOR_CLASSES; i++)
			colorKeys += "color" + std::to_string(i) + ", ";
		string colorValues = "";
		for (int i = 0; i < NUM_OF_COLOR_CLASSES; i++)
			colorValues += std::to_string(colorRatios[i]).substr(0,5) + ", ";
		sprintf(query, "INSERT INTO objectInfo (videoId, objectId, classId, %s %s speed) VALUES (%d, %d, %d, %s %s %8lf);", directionKeys.c_str(), colorKeys.c_str(), videoId, objectId, classId, directionValues.c_str(), colorValues.c_str(), speed);
		insert(query);
	}
};