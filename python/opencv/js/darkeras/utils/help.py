import pymysql
import os

def say(*words, verbalise=False):
	if verbalise:
		print(list(words))

def convert_darkweights2keras(model, weigths_path, verbalise=False):
	data = np.fromfile(weights_path, np.float32)
	data = data[4:]
	say("weights shape : ", data.shape, verbalise=verbalise)
	idx = 0
	for i,layer in enumerate(model.layers):
		shape = [w.shape for w in layer.get_weights()]
		if shape != []:
			kshape,bshape = shape
			bia = data[idx:idx+np.prod(bshape)].reshape(bshape)
			idx += np.prod(bshape)
			ker = data[idx:idx+np.prod(kshape)].reshape(kshape)
			idx += np.prod(kshape)
			layer.set_weights([ker,bia])
	say("convert np weights file -> kears models", "Successful", verbalise=verbalise)

def conv_weigths_flatten(layer_weights_comp):
	flatten = []
	for sub_1 in layer_weights_comp:
		for sub_2 in sub_1:
			for sub_3 in sub_2:
				for val in sub_3:
					flatten.append(val)
	return flatten

class DB_Item:
	def __init__(self, videoId, frameNum, _class, x, y, w, h):
		self.videoId = videoId
		self.frameNum = frameNum
		self._class = _class
		self.x, self.y, self.w, self.h = x, y, w, h

	def to_list(self):
		return [self.videoId, self.frameNum, self._class, self.x, self.y, self.w, self.h]

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
		print("Open DB..")

	def insert(self, items, table_name=None):
		if table_name is None:
			table_name = self.curr_table_name
		
		# if table column change, it also need to change for sequence by changed table.
		sql = 'insert into '+table_name+' values (NULL, {}, {}, {}, {}, {}, {}, {})'
		for each_item in items:
			self.curs.execute(sql.format(*each_item.to_list()))
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

# Model History Save Modules in RapidCheck Darkeras
def save_model(model, steps, save_folder, save_type='weights'):
	"""
		model : keras model
		save_type : all -> weights + full model
					weights -> only weights save
					model -> only model save 
	"""
	if save_type == 'weights':
		model.save_weights(trained_save_weights_prefix + 'steps{}.h5'.format(steps))
		json_arch = model.to_json()
		# with open(os.path.join(save_folder))
	elif save_type == 'model':
		model.save(trained_save_weights_prefix + 'steps{}.h5'.format(steps))
	elif save_type == 'all':
		model.save_weights(trained_save_weights_prefix + 'steps{}.h5'.format(steps))
		model.save(trained_save_weights_prefix + 'steps{}.h5'.format(steps))

if __name__ == '__main__':
	s = "dfkjasidfajfl\
								dkfjrikdsflkjefdsfliejsdfl\n\
				asdfj"
	print(s)