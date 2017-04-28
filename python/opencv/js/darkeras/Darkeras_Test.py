
# coding: utf-8

# In[1]:

import os
import glob
import h5py
import numpy as np
import cv2
import matplotlib.pyplot as plt
from keras import backend as K

import keras
from keras.models import Sequential
from keras.layers.convolutional import Conv2D, MaxPooling2D
from keras.layers.advanced_activations import LeakyReLU
from keras.layers.core import Flatten, Dense, Activation, Reshape

import tensorflow as tf
import yolo.config as cfg


# In[2]:

keras.backend.set_image_dim_ordering('th')
weights_path = 'yolo-tiny-origin.h5'
is_freeze = True


# In[3]:

model = Sequential()
model.add(Conv2D(16, (3, 3), input_shape=(3,448,448),padding='same', 
                        activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Conv2D(32,(3,3), padding='same', 
                        activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))
model.add(Conv2D(64,(3,3), padding='same', 
                        activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))
model.add(Conv2D(128,(3,3), padding='same', 
                        activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))
model.add(Conv2D(256,(3,3), padding='same', 
                        activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))
model.add(Conv2D(512,(3,3), padding='same', 
                        activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))
model.add(Conv2D(1024,(3,3), padding='same', activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(Conv2D(1024,(3,3), padding='same', activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(Conv2D(1024,(3,3), padding='same', activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(Flatten())
model.add(Dense(256))
model.add(Dense(4096))
model.add(LeakyReLU(alpha=0.1))
model.add(Dense(1470))


# ## Test original weights file

# In[4]:

model.load_weights(weights_path)


# In[5]:

model.summary()


# In[6]:

from utils.BoxUtils import post_progress


# In[7]:

# imagePath = './test/my_testset/001618.jpg'
# imagePath = './test/my_testset/000892.jpg'
# imagePath = './test/my_testset/000906.jpg'
# imagePath = './test/my_testset/000467.jpg'
imagePath = './test/my_testset/000386.jpg'
# imagePath = './test/my_testset/person.jpg'
# imagePath = './test/my_testset/person.jpg'
image = cv2.imread(imagePath)
print("1", image.shape)
resized = cv2.resize(image,(448,448))
plt.imshow(resized)
print("2", resized.shape)
np_img = cv2.cvtColor(resized, cv2.COLOR_BGR2RGB)
batch = np.transpose(np_img,(2,0,1))
print("3", batch.shape)
batch = 2*(batch/255.) - 1
batch = np.expand_dims(batch, axis=0)
print("4", batch.shape)


# In[8]:

out = model.predict(batch)
print("5", out.shape)


# In[9]:

out_img = post_progress(out[0], im=image, is_save=False, threshold=0.1)
print("6", out_img.shape)
out_img = cv2.cvtColor(out_img, cv2.COLOR_BGR2RGB)
plt.imshow(out_img)
plt.show()

# ## Test step1 save weights file

# In[18]:


weigths_path2 = 'yolo-tiny2-epoch10.h5'
test_threshold = 0.05

model.load_weights(weigths_path2)
model.summary()


# In[24]:

# imagePath = './test/my_testset/person.jpg'
image = cv2.imread(imagePath)
# print("1", image.shape)
# resized = cv2.resize(image,(448,448))
# plt.imshow(resized)
# print("2", resized.shape)
# np_img = cv2.cvtColor(resized, cv2.COLOR_BGR2RGB)
# batch = np.transpose(np_img,(2,0,1))
# print("3", batch.shape)
# batch = 2*(batch/255.) - 1
# batch = np.expand_dims(batch, axis=0)
# print("4", batch.shape)
out = model.predict(batch)
print("5", out.shape)


out_img = post_progress(out[0], im=image, is_save=False, threshold=test_threshold)
print("6", out_img.shape)
out_img = cv2.cvtColor(out_img, cv2.COLOR_BGR2RGB)
plt.imshow(out_img)
plt.show()

# 

weigths_path2 = 'yolo-tiny3-epoch2.h5'
model.load_weights(weigths_path2)
image = cv2.imread(imagePath)
out = model.predict(batch)
print("5", out.shape)
out_img = post_progress(out[0], im=image, is_save=False, threshold=test_threshold)
print("6", out_img.shape)
out_img = cv2.cvtColor(out_img, cv2.COLOR_BGR2RGB)
plt.imshow(out_img)
plt.show()

weigths_path2 = 'yolo-tiny4-epoch0.h5'
model.load_weights(weigths_path2)
image = cv2.imread(imagePath)
out = model.predict(batch)
print("5", out.shape)
out_img = post_progress(out[0], im=image, is_save=False, threshold=test_threshold)
print("6", out_img.shape)
out_img = cv2.cvtColor(out_img, cv2.COLOR_BGR2RGB)
plt.imshow(out_img)
plt.show()

weigths_path2 = 'yolo-tiny4-epoch1.h5'
model.load_weights(weigths_path2)
image = cv2.imread(imagePath)
out = model.predict(batch)
print("5", out.shape)
out_img = post_progress(out[0], im=image, is_save=False, threshold=test_threshold)
print("6", out_img.shape)
out_img = cv2.cvtColor(out_img, cv2.COLOR_BGR2RGB)
plt.imshow(out_img)
plt.show()

weigths_path2 = 'yolo-tiny4-epoch2.h5'
model.load_weights(weigths_path2)
image = cv2.imread(imagePath)
out = model.predict(batch)
print("5", out.shape)
out_img = post_progress(out[0], im=image, is_save=False, threshold=test_threshold)

print("6", out_img.shape)
out_img = cv2.cvtColor(out_img, cv2.COLOR_BGR2RGB)
plt.imshow(out_img)
plt.show()

weigths_path2 = 'yolo-tiny4-epoch3.h5'
model.load_weights(weigths_path2)
image = cv2.imread(imagePath)
out = model.predict(batch)
print("5", out.shape)
out_img = post_progress(out[0], im=image, is_save=False, threshold=test_threshold)
print("6", out_img.shape)
out_img = cv2.cvtColor(out_img, cv2.COLOR_BGR2RGB)
plt.imshow(out_img)
plt.show()




