
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
from utils.help import say, convert_darkweights2keras


# In[2]:

sess = tf.Session()
K.set_session(sess)

keras.backend.set_image_dim_ordering('th')
pretrained_weights_path = 'models/pretrain/yolo-tiny-origin-named.h5'
is_freeze = True
verbalise = True

freeze_layer_weights = None
trainable_layer_weights = None
show_trainable_state = False # 여기를 True 로 바꾸면, conv layer 와 dense layer 의 학습별 weigths 가 변하는지 안변하는지를 확인할 수 있다.
trained_save_weights_prefix = 'models/train/{}-'.format(cfg.model_name)

inp_w, inp_h, inp_c = cfg.inp_size
output_tensor_shape = (cfg.cell_size * cfg.cell_size)*(cfg.num_classes + cfg.boxes_per_cell*5)

# In[3]:

model = Sequential()
model.add(Conv2D(16, (3, 3), input_shape=(inp_c, inp_w, inp_h),padding='same', 
                        activation=LeakyReLU(alpha=0.1), trainable=not is_freeze, name='conv1'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Conv2D(32,(3,3), padding='same', 
                        activation=LeakyReLU(alpha=0.1), trainable=not is_freeze, name='conv2'))
model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))
model.add(Conv2D(64,(3,3), padding='same', 
                        activation=LeakyReLU(alpha=0.1), trainable=not is_freeze, name='conv3'))
model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))
model.add(Conv2D(128,(3,3), padding='same', 
                        activation=LeakyReLU(alpha=0.1), trainable=not is_freeze, name='conv4'))
model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))
model.add(Conv2D(256,(3,3), padding='same', 
                        activation=LeakyReLU(alpha=0.1), trainable=not is_freeze, name='conv5'))
model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))
model.add(Conv2D(512,(3,3), padding='same', 
                        activation=LeakyReLU(alpha=0.1), trainable=not is_freeze, name='conv6'))
model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))
model.add(Conv2D(1024,(3,3), padding='same', activation=LeakyReLU(alpha=0.1), trainable=not is_freeze, name='conv7'))
model.add(Conv2D(1024,(3,3), padding='same', activation=LeakyReLU(alpha=0.1), trainable=not is_freeze, name='conv8'))
model.add(Conv2D(1024,(3,3), padding='same', activation=LeakyReLU(alpha=0.1), trainable=not is_freeze, name='conv9'))
model.add(Flatten())
model.add(Dense(256, name='new_dense1'))
model.add(Dense(4096, name='new_dense2'))
model.add(LeakyReLU(alpha=0.1))
model.add(Dense(output_tensor_shape, name='new_detection'))


# In[4]:

model.summary()


# In[5]:

from yolo.training_v1 import darkeras_loss, _TRAINER
from yolo.datacenter.data import shuffle


# In[6]:

inp_x = model.input
net_out = model.output


say("Building {} loss function".format(cfg.model_name), verbalise=verbalise)
loss_ph, loss_op = darkeras_loss(net_out)
say("Building {} train optimizer".format(cfg.model_name), verbalise=verbalise)
optimizer = _TRAINER[cfg.trainer](cfg.lr)
gradients = optimizer.compute_gradients(loss_op, var_list=model.trainable_weights)
train_op = optimizer.apply_gradients(gradients)

sess.run(tf.global_variables_initializer())

conv1 = model.layers[0]
dense_last = model.layers[-1]
print("conv1 : ")
print(sess.run(conv1.weights[0])[0][0][0])
print("danse last : ")
print(sess.run(dense_last.weights[0])[0][0:10])


# In[7]:

model.load_weights(pretrained_weights_path, by_name=True)


# In[8]:

conv1 = model.layers[0]
dense_last = model.layers[-1]
print("conv1 : ")
print(sess.run(conv1.weights[0])[0][0][0])
print("danse last : ")
print(sess.run(dense_last.weights[0])[0][0:10])


# In[9]:

# from utils.BoxUtils import post_progress


# In[10]:

# imagePath = './test/my_testset/person.jpg'
# image = cv2.imread(imagePath)
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

# out = model.predict(batch)
# print("5", out.shape)

# out_img = post_progress(out[0], im=image, is_save=False, threshold=0.1)
# print("6", out_img.shape)
# out_img = cv2.cvtColor(out_img, cv2.COLOR_BGR2RGB)
# plt.imshow(out_img)


# ## 오케이. new-layer 부분은 깨끗하게 Initalized 된 것을 확인하였다.

# In[23]:

def conv_weigths_flatten(layer_weights_comp):
    flatten = []
    for sub_1 in layer_weights_comp:
        for sub_2 in sub_1:
            for sub_3 in sub_2:
                for val in sub_3:
                    flatten.append(val)
    return flatten


# In[24]:



batches = shuffle()
for i, (x_batch, datum) in enumerate(batches):
    train_feed_dict = {
       loss_ph[key]:datum[key] for key in loss_ph 
    }
    train_feed_dict[inp_x] = x_batch
    # print("feed_dict.keys() : ", len(train_feed_dict.keys()), train_feed_dict.keys())
    fetches = [train_op, loss_op] 
    fetched = sess.run(fetches, feed_dict=train_feed_dict)
    
    loss_val = fetched[1]
    say("step {} - loss {}".format(i, loss_val), verbalise=True)
    
    if show_trainable_state:
        conv1 = model.layers[0]
        dense_last = model.layers[-1]
        if freeze_layer_weights is None:
            freeze_layer_weights = sess.run(conv1.weights[0])
            trainable_layer_weights = sess.run(dense_last.weights[0])
        else:
            freeze_layer_comp = (freeze_layer_weights == sess.run(conv1.weights[0]))
            print("conv1 : ", conv1.name, all(conv_weigths_flatten(freeze_layer_comp)))

            last_layer_comp = (trainable_layer_weights == sess.run(dense_last.weights[0]))
            last_layer_flatten = [val for sublist in last_layer_comp for val in sublist]
            print("danse last : ", dense_last.name, all(last_layer_flatten))
    if i == 1:
        model.save_weights(trained_save_weights_prefix + 'epoch{}.h5'.format(i))
        say("Save weights : ", trained_save_weights_prefix + 'epoch{}.h5'.format(i), verbalise=verbalise)
   
    if i % 3100 == 0:
        model.save_weights(trained_save_weights_prefix + 'epoch{}.h5'.format(i//310))
        say("Save weights : ", trained_save_weights_prefix + 'epoch{}.h5'.format(i//310), verbalise=verbalise)

say('Training All Done..', verbalise=verbalise)
model.save_weights(trained_save_weights_prefix + 'complete.h5')
say("Save weights : ", trained_save_weights_prefix + 'complete.h5', verbalise=verbalise)


# In[ ]:



