import pymysql
from uitls.help import DB_Helper, DB_Item

if __name__ == '__main__':
	db = DB_Helper()
	db.open()
	items = []
	db_item = DB_Item(2,3,4,5,6,7,8)
	items.append(db_item)
	db.insert(items)
	# db.delete()
	db.select()
	db.close()
	exit()
