import pymysql
import os
import json
import matplotlib.pyplot as plt
import numpy as np
import sys

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
	def __init__(self, videoId, frameNum, _class, x, y, w, h, confidence):
		self.videoId = videoId
		self.frameNum = frameNum
		self._class = _class
		self.x, self.y, self.w, self.h = x, y, w, h
		self.confidence = confidence

	def to_list(self):
		return [self.videoId, self.frameNum, self._class, self.x, self.y, self.w, self.h, self.confidence]

class DB_Helper:
	curr_table_name = 'detection'
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
		sql = 'insert into '+table_name+' (videoId, frameNum, classId, x, y, width, height, confidence) values ({}, {}, {}, {}, {}, {}, {}, {});'
		for each_item in items:
			self.curs.execute(sql.format(*each_item.to_list()))
		self.conn.commit()

	def select(self, table_name=None, condition=None):
		if table_name is None:
			table_name = self.curr_table_name
		
		sql = "select * from "+table_name
		if condition:
			for key in condition:
				sql += ' where {}={}'.format(key, condition[key])

		self.curs.execute(sql)
 
		# Data Fetch
		rows = self.curs.fetchall()
		rows = rows[0]
		# print(type(rows), rows) # <class 'tuple'> ((4, 'C:videoscctv5.mp4', 5, 0, None),)
		return {'videoId':rows[0], 'videoPath':rows[1], 'frameSteps':rows[2], 'status':rows[3]}

	def delete(self, table_name=None):
		if table_name is None:
			table_name = self.curr_table_name
		
		sql = 'delete from '+table_name
		self.curs.execute(sql)

	def close(self):
		self.conn.close()

# Model History Save Modules in RapidCheck Darkeras
def save_model(model, save_folder, file_name, steps, descriptions, save_type='weights'):
	"""
		model : keras model
		save_type : all -> weights + full model
					weights -> only weights save
					model -> only model save 
	"""
	if save_type == 'weights':
		model.save_weights(os.path.join(save_folder, file_name) + '-steps{}.h5'.format(steps))
		print("Model Save Weights steps : {} ".format(steps))
		architecture_path = os.path.join(save_folder, file_name) + '_arch.json'
		# if not os.path.exists(architecture_path):
		# 	# 모델의 구조에 대한 내용이 없을 경우에만 내리도록 한다.
		# 	json_arch = model.to_json()
		# 	with open(architecture_path, 'w+') as f:
		# 		json.dumps(json_arch, f)
		# 		print("Model Architecture Infomation saved in : ", architecture_path)
		descriptions_path = os.path.join(save_folder, file_name) + '_desc.txt'
		if not os.path.exists(descriptions_path):
			# 모델에 대한 설명을 적은 파일이 존재하지 않을때만 생성 및 작성한다.
			with open(descriptions_path, 'w+') as f:
				f.write(descriptions)
				print("Model Descriptions Infomation saved in : ", descriptions_path)
	elif save_type == 'model':
		model.save(os.path.join(save_folder, file_name) + '-steps{}.h5'.format(steps))
	elif save_type == 'all':
		model.save_weights(os.path.join(save_folder, file_name) + '-steps{}.h5'.format(steps))
		model.save(os.path.join(save_folder, file_name) + '-steps{}.h5'.format(steps))


def plot_model_history(model_history, save_path, model_name, record_step=100, is_test=False):
	fig, axs = plt.subplots(1,1,figsize=(10,5))
	axs.plot(range(1,len(model_history['train_loss'])*record_step+1, record_step), model_history['train_loss'])
	if is_test:
		axs.plot(range(1,len(model_history['val_loss'])*record_step+1, record_step), model_history['val_loss'])
	axs.set_title('Model Loss')
	axs.set_ylabel('loss')
	axs.set_xlabel('Steps')
	axs.set_xticks(np.arange(1,len(model_history['train_loss'])+1),len(model_history['train_loss'])/10)
	axs.legend(['train_loss', 'val_loss'], loc='best')
	# plt.show()
	fig.savefig(os.path.join(save_path, model_name+'_LossGraph'+'.png'))


if __name__ == '__main__':
	s = "dfkjasidfajfl\
								dkfjrikdsflkjefdsfliejsdfl\n\
				asdfj"
	print(s)