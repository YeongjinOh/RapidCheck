# coding: utf-8

# In[1]:

import os
import glob
import h5py
import numpy as np
import cv2
import matplotlib.pyplot as plt
from keras import backend as K

import tensorflow as tf
import yolo.config as cfg
from yolo.net.yolo_tiny_tfdim_net import yolo_tiny_TFdim_model
# from yolo.net.vgg16_net import yolo_vgg16_TFdim_model

# In[2]:


# weights_path = 'models/train/yolo-tiny-tfdim-new-steps10000.h5'
weights_path = 'models/pretrain/yolo-tiny-origin-tfdim-named.h5'
# weights_path = 'models/train/yolo-vgg16-2class-steps40000.h5'
is_freeze = True
output_tensor_shape = (cfg.cell_size * cfg.cell_size)*(cfg.num_classes + cfg.boxes_per_cell*5)

# model = yolo_vgg16_TFdim_model(is_top=True, is_new_training=True)
model = yolo_tiny_TFdim_model()
model.summary()
model.load_weights(weights_path)

from utils.BoxUtils import post_progress
from yolo.process import preprocess

threshold = 0.2


# imagePath = './test/my_testset/person_cow.jpg'
# imagePath = './test/my_testset/000892.jpg'
# imagePath = './test/my_testset/000906.jpg'
# imagePath = './test/my_testset/000467.jpg'
# imagePath = './test/my_testset/000386.jpg'
# imagePath = './test/my_testset/many_person.jpg'
# imagePath = './test/my_testset/person.jpg'
# imagePath = './test/my_testset/apart_car_test.jpg'
# imagePath = './test/my_testset/person_car3.jpg'
# imagePath = './test/my_testset/car1.jpg'
imagePath = './test/my_testset/car2.jpg'
test_weights_list = [weights_path]
# test_weights_list = ['models/train/yolo-vgg16-2class-steps10000.h5',
# 					'models/train/yolo-vgg16-2class-steps15000.h5',
# 					'models/train/yolo-vgg16-2class-steps20000.h5',
# 					'models/train/yolo-vgg16-2class-steps25000.h5',
# 					'models/train/yolo-vgg16-2class-steps30000.h5',
# 					'models/train/yolo-vgg16-2class-steps35000.h5',
# 					'models/train/yolo-vgg16-2class-steps40000.h5',
# 					'models/train/yolo-vgg16-2class-steps45000.h5',
# 					'models/train/yolo-vgg16-2class-steps50000.h5',
# 					'models/train/yolo-vgg16-2class-steps55000.h5',
# 					'models/train/yolo-vgg16-2class-steps60000.h5',
# 					'models/train/yolo-vgg16-2class-steps65000.h5',
# 					'models/train/yolo-vgg16-2class-steps70000.h5',
# 					'models/train/yolo-vgg16-2class-steps75000.h5',
# 					'models/train/yolo-vgg16-2class-steps80000.h5']


for each_weight_path in test_weights_list:
	model.load_weights(each_weight_path)
	
	img = cv2.imread(imagePath)
	print("1", img.shape)
	image = preprocess(img)
	batch = np.expand_dims(image, axis=0)
	print("4", batch.shape)
	out = model.predict(batch)
	print("5", out.shape)
	out_img = post_progress(out[0], im=img, is_save=False, threshold=threshold)
	print("6", out_img.shape)
	out_img = cv2.cvtColor(out_img, cv2.COLOR_BGR2RGB)
	plt.imshow(out_img)
	plt.show()
