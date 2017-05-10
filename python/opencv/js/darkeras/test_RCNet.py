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

# In[2]:

weights_path = 'models/train/yolo-tiny-tfdim-new-steps70000.h5'
is_freeze = True
output_tensor_shape = (cfg.cell_size * cfg.cell_size)*(cfg.num_classes + cfg.boxes_per_cell*5)

model = yolo_tiny_TFdim_model()
model.summary()
model.load_weights(weights_path)

from utils.BoxUtils import post_progress
from yolo.process import preprocess

# imagePath = './test/my_testset/001618.jpg'
# imagePath = './test/my_testset/000892.jpg'
# imagePath = './test/my_testset/000906.jpg'
# imagePath = './test/my_testset/000467.jpg'
# imagePath = './test/my_testset/000386.jpg'
# imagePath = './test/my_testset/many_person.jpg'
# imagePath = './test/my_testset/person.jpg'
# imagePath = './test/my_testset/apart_car_test.jpg'
# imagePath = './test/my_testset/person_car3.jpg'
imagePath = './test/my_testset/car1.jpg'

image = cv2.imread(imagePath)
print("1", image.shape)
image = preprocess(image)
batch = np.expand_dims(image, axis=0)
print("4", batch.shape)


# # In[8]:

out = model.predict(batch)
print("5", out.shape)


# # In[9]:

out_img = post_progress(out[0], im=image, is_save=False, threshold=0.1)
print("6", out_img.shape)
out_img = cv2.cvtColor(out_img, cv2.COLOR_BGR2RGB)
plt.imshow(out_img)
plt.show()