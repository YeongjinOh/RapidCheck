import pymysql

class DB_Helper:
	
	curr_table_name = 'detection2'
	
	def __init__(self, conn=None, curs=None):
		self.conn = conn # db connection
		self.curs = curs # db cursor

	def open(self, user='root', password='1234', db_name='rapidcheck', charset='utf8', table_name='detection2'):
		# MySQL Connection 연결
		self.conn = pymysql.connect(host='localhost', user=user, password=password,
						db=db_name, charset=charset)
		self.curs = self.conn.cursor() # get cursor 
		self.curr_table_name = table_name # using table name

	def insert(self, items, table_name=None):
		if table_name is None:
			table_name = self.curr_table_name
		
		# if table column change, it also need to change for sequence by changed table.
		sql = 'insert into '+table_name+' values (NULL, {}, {}, {}, {}, {}, {}, {})'
		for each_item in items:
			print(each_item)
			self.curs.execute(sql.format(*each_item))
		self.conn.commit()

	def select(self, table_name=None, condition=None):
		if table_name is None:
			table_name = self.curr_table_name

		sql = "select * from "+table_name
		self.curs.execute(sql)
 
		# Data Fetch
		rows = self.curs.fetchall()
		print(type(rows), rows)

	def delete(self, table_name=None):
		if table_name is None:
			table_name = self.curr_table_name
		
		sql = 'delete from '+table_name
		self.curs.execute(sql)

	def close(self):
		self.conn.close()


if __name__ == '__main__':
	db = DB_Helper()
	db.open()
	items = [
	    [1,1,1,1,1,1,1],
	    [1,2,2,2,2,2,1],
	    [1,3,3,3,3,3,2]
	]
	db.insert(items)
	# db.delete()
	db.select()
	db.close()
	exit()
