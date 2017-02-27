import glob
import os

train_img_path = './dataset/CarData/TrainImages/'
pos_img_cnt = 0 
neg_img_cnt = 0

pos_train_dir_path = './dataset/CarData/TrainImages/pos'
neg_train_dir_path = './dataset/CarData/TrainImages/neg'

if not os.path.isdir(pos_train_dir_path):
	os.makedirs(pos_train_dir_path)
	print("Making %s directory.." % pos_train_dir_path)

if not os.path.isdir(neg_train_dir_path):
	os.makedirs(neg_train_dir_path)
	print("Making %s directory.." % neg_train_dir_path)

for im_path in glob.glob(os.path.join(train_img_path, "*")):
	img_name = im_path.split('/')[-1]
	# print(img_name)
	class_ = img_name.split('-')[0]
	if class_ == 'pos':
		pos_img_cnt += 1
	elif class_ == 'neg':
		neg_img_cnt += 1
print("pos cnt ", pos_img_cnt)
print("neg cnt ", neg_img_cnt)