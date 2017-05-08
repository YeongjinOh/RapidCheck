import numpy as np
import keras
from keras.models import Sequential
from keras.layers.convolutional import Conv2D, MaxPooling2D
from keras.layers.advanced_activations import LeakyReLU
from keras.layers.core import Flatten, Dense, Activation, Reshape

import tensorflow as tf
import yolo.config as cfg

## Make VGG16 networks for feature selection
pretrained_weights_path = 'models/pretrain/vgg16_weights.h5'

def VGG16_network():
	model_vgg = Sequential()
	model_vgg.add(ZeroPadding2D((1, 1), input_shape=(img_width, img_height,3)))
	model_vgg.add(Convolution2D(64, 3, 3, activation='relu', name='conv1_1'))
	model_vgg.add(ZeroPadding2D((1, 1)))
	model_vgg.add(Convolution2D(64, 3, 3, activation='relu', name='conv1_2'))
	model_vgg.add(MaxPooling2D((2, 2), strides=(2, 2)))

	model_vgg.add(ZeroPadding2D((1, 1)))
	model_vgg.add(Convolution2D(128, 3, 3, activation='relu', name='conv2_1'))
	model_vgg.add(ZeroPadding2D((1, 1)))
	model_vgg.add(Convolution2D(128, 3, 3, activation='relu', name='conv2_2'))
	model_vgg.add(MaxPooling2D((2, 2), strides=(2, 2)))

	model_vgg.add(ZeroPadding2D((1, 1)))
	model_vgg.add(Convolution2D(256, 3, 3, activation='relu', name='conv3_1'))
	model_vgg.add(ZeroPadding2D((1, 1)))
	model_vgg.add(Convolution2D(256, 3, 3, activation='relu', name='conv3_2'))
	model_vgg.add(ZeroPadding2D((1, 1)))
	model_vgg.add(Convolution2D(256, 3, 3, activation='relu', name='conv3_3'))
	model_vgg.add(MaxPooling2D((2, 2), strides=(2, 2)))

	model_vgg.add(ZeroPadding2D((1, 1)))
	model_vgg.add(Convolution2D(512, 3, 3, activation='relu', name='conv4_1'))
	model_vgg.add(ZeroPadding2D((1, 1)))
	model_vgg.add(Convolution2D(512, 3, 3, activation='relu', name='conv4_2'))
	model_vgg.add(ZeroPadding2D((1, 1)))
	model_vgg.add(Convolution2D(512, 3, 3, activation='relu', name='conv4_3'))
	model_vgg.add(MaxPooling2D((2, 2), strides=(2, 2)))

	model_vgg.add(ZeroPadding2D((1, 1)))
	model_vgg.add(Convolution2D(512, 3, 3, activation='relu', name='conv5_1'))
	model_vgg.add(ZeroPadding2D((1, 1)))
	model_vgg.add(Convolution2D(512, 3, 3, activation='relu', name='conv5_2'))
	model_vgg.add(ZeroPadding2D((1, 1)))
	model_vgg.add(Convolution2D(512, 3, 3, activation='relu', name='conv5_3'))
	model_vgg.add(MaxPooling2D((2, 2), strides=(2, 2)))

	f = h5py.File(pretrained_weights_path)
	for k in range(f.attrs['nb_layers']):
		if k >= len(model_vgg.layers) - 1:
		# we don't look at the last two layers in the savefile (fully-connected and activation)
			break
		g = f['layer_{}'.format(k)]
		weights = [g['param_{}'.format(p)] for p in range(g.attrs['nb_params'])]
		layer = model_vgg.layers[k]

		if layer.__class__.__name__ in ['Convolution1D', 'Convolution2D', 'Convolution3D', 'AtrousConvolution2D']:
			weights[0] = np.transpose(weights[0], (2, 3, 1, 0))

		layer.set_weights(weights)
	f.close()
	print("VGG16 Model with No Top loaded...")
	return model_vgg