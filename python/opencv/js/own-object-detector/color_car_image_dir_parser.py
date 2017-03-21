import glob
import os
from shutil import copyfile, rmtree
import argparse as ap
# python3 color_car_image_dir_parser.py -s 150150

parser = ap.ArgumentParser()
parser.add_argument('-s', "--size", help="window size", required=True)
args = vars(parser.parse_args())


train_img_path = './dataset/ColorCars/TrainResized{}/'.format(args['size'])
bg_img_path = './dataset/ColorCars/BackgroundResized{}/'.format(args['size'])

train_dir_path = './dataset/ColorCars/Trains{}/'.format(args['size'])
validation_dir_path = './dataset/ColorCars/Validations{}/'.format(args['size'])
pos_img_cnt = 0 
neg_img_cnt = 0

pos_train_dir_path = train_dir_path + 'pos'
neg_train_dir_path = train_dir_path + 'neg'

pos_validation_dir_path = validation_dir_path + 'pos'
neg_validation_dir_path = validation_dir_path + 'neg'

dir_paths = [pos_train_dir_path, neg_train_dir_path, 
			pos_validation_dir_path, neg_validation_dir_path]

nb_train_samples = 400
nb_validation_samples = 100
nb_epoch = 50

for each_path in dir_paths:
	if os.path.isdir(each_path):
		rmtree(each_path)
		os.makedirs(each_path)
		print("Clean %s directory.." % each_path)
	elif not os.path.isdir(each_path):
		os.makedirs(each_path)
		print("Making %s directory.." % each_path)

print(os.listdir(train_dir_path))

for im_path in glob.glob(os.path.join(train_img_path, "*")):
	img_name = im_path.split('/')[-1]
	# print(img_name)
	if pos_img_cnt < nb_train_samples:
		copyfile(im_path, pos_train_dir_path+'/'+img_name)
	else:
		if pos_img_cnt > nb_train_samples + nb_validation_samples:
			continue
		copyfile(im_path, pos_validation_dir_path+'/'+img_name)
	pos_img_cnt += 1

for im_path in glob.glob(os.path.join(bg_img_path, "*")):
	img_name = im_path.split('/')[-1]
	# print(img_name)
	if neg_img_cnt < nb_train_samples:
		copyfile(im_path, neg_train_dir_path+'/'+img_name)
	else:
		if neg_img_cnt > nb_train_samples + nb_validation_samples:
			continue
		copyfile(im_path, neg_validation_dir_path + '/'+ img_name)
	neg_img_cnt += 1

print(len(os.listdir(os.path.join(train_dir_path, 'pos'))))
print(len(os.listdir(os.path.join(train_dir_path, 'neg'))))
print(len(os.listdir(os.path.join(validation_dir_path, 'pos'))))
print(len(os.listdir(os.path.join(validation_dir_path, 'neg'))))

print("pos cnt ", pos_img_cnt)
print("neg cnt ", neg_img_cnt)