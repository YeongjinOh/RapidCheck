import glob
import os
from PIL import Image

train_img_path = 'dataset/CarData/TrainImages/'
train_dir_path = 'dataset/CarData/Trains/'
validation_dir_path = 'dataset/CarData/Validations/'
pos_img_cnt = 0 
neg_img_cnt = 0

pos_train_dir_path = train_dir_path + 'pos'
neg_train_dir_path = train_dir_path + 'neg'

pos_validation_dir_path = validation_dir_path + 'pos'
neg_validation_dir_path = validation_dir_path + 'neg'

dir_paths = [pos_train_dir_path, neg_train_dir_path, 
			pos_validation_dir_path, neg_validation_dir_path]

for dir_path in dir_paths:
	for im_path in glob.glob(os.path.join(dir_path, "*")):
		print(">> Converting : ", im_path)
		file_name = im_path.split('.')[0]
		# print(file_name)
		im = Image.open(im_path)
		im.save(file_name+'.jpg')
		os.remove(im_path)

print('Convert format done..')
