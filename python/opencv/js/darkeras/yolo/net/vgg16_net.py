import numpy as np
import keras
from keras.models import Sequential
from keras.layers.convolutional import Conv2D, MaxPooling2D, ZeroPadding2D
from keras.layers.advanced_activations import LeakyReLU
from keras.layers.core import Flatten, Dense, Activation, Reshape, Dropout

import tensorflow as tf
import yolo.config as cfg

## Make VGG16 networks for feature selection
pretrained_weights_path = 'models/pretrain/vgg16_tfdim_top_weights.h5'
img_width, img_height, img_channel = cfg.inp_size
def yolo_vgg16_TFdim_model(is_top=False):
	model_vgg = Sequential()
	model_vgg.add(Conv2D(64, (3, 3), padding='same', input_shape=(img_width, img_height,img_channel),
					activation='relu', name='conv1_1', trainable=False))
	model_vgg.add(Conv2D(64, (3, 3), padding='same', activation='relu', name='conv1_2', trainable=False))
	model_vgg.add(MaxPooling2D((2, 2), strides=(2, 2)))

	model_vgg.add(Conv2D(128, (3, 3), padding='same', activation='relu', name='conv2_1', trainable=False))
	model_vgg.add(Conv2D(128, (3, 3), padding='same', activation='relu', name='conv2_2', trainable=False))
	model_vgg.add(MaxPooling2D((2, 2), strides=(2, 2)))

	model_vgg.add(Conv2D(256, (3, 3), padding='same', activation='relu', name='conv3_1', trainable=False))
	model_vgg.add(Conv2D(256, (3, 3), padding='same', activation='relu', name='conv3_2', trainable=False))
	model_vgg.add(Conv2D(256, (3, 3), padding='same', activation='relu', name='conv3_3', trainable=False))
	model_vgg.add(MaxPooling2D((2, 2), strides=(2, 2)))

	model_vgg.add(Conv2D(512, (3, 3), padding='same', activation='relu', name='conv4_1', trainable=False))
	model_vgg.add(Conv2D(512, (3, 3), padding='same', activation='relu', name='conv4_2', trainable=False))
	model_vgg.add(Conv2D(512, (3, 3), padding='same', activation='relu', name='conv4_3', trainable=False))
	model_vgg.add(MaxPooling2D((2, 2), strides=(2, 2)))

	model_vgg.add(Conv2D(512, (3, 3), padding='same', activation='relu', name='conv5_1', trainable=False))
	model_vgg.add(Conv2D(512, (3, 3), padding='same', activation='relu', name='conv5_2', trainable=False))
	model_vgg.add(Conv2D(512, (3, 3), padding='same', activation='relu', name='conv5_3', trainable=False))
	model_vgg.add(MaxPooling2D((2, 2), strides=(2, 2)))

	if is_top:
		model_vgg.add(Flatten())
		model_vgg.add(Dense(4096, activation='relu', name='dense_1'))
		model_vgg.add(Dropout(0.5))
		model_vgg.add(Dense(4096, activation='relu', name='dense_2'))
		model_vgg.add(Dropout(0.5))
		model_vgg.add(Dense(1000, activation='softmax', name='dense_3'))
	else:
		pass

	return model_vgg