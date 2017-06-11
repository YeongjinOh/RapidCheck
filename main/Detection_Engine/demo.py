import os
import glob
import h5py
import numpy as np
import cv2
import matplotlib.pyplot as plt

from keras import backend as K
import keras
from keras.models import Sequential
from keras.layers.convolutional import Convolution2D, MaxPooling2D
from keras.layers.advanced_activations import LeakyReLU
from keras.layers.core import Flatten, Dense, Activation, Reshape


from yolo.dataset.data import parse, shuffle

print("Hello, This is darkeras..!")

# Set Keras backend settings, weights files
keras.backend.set_image_dim_ordering('th')
weights_path = 'yolo-tiny.weights'
is_freeze = True

# Build yolo tiny networks
model = Sequential()
model.add(Convolution2D(16, 3, 3,input_shape=(3,448,448),border_mode='same',subsample=(1,1), 
						activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Convolution2D(32,3,3 ,border_mode='same', 
						activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(MaxPooling2D(pool_size=(2, 2),border_mode='valid'))
model.add(Convolution2D(64,3,3 ,border_mode='same', 
						activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(MaxPooling2D(pool_size=(2, 2),border_mode='valid'))
model.add(Convolution2D(128,3,3 ,border_mode='same', 
						activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(MaxPooling2D(pool_size=(2, 2),border_mode='valid'))
model.add(Convolution2D(256,3,3 ,border_mode='same', 
						activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(MaxPooling2D(pool_size=(2, 2),border_mode='valid'))
model.add(Convolution2D(512,3,3 ,border_mode='same', 
						activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(MaxPooling2D(pool_size=(2, 2),border_mode='valid'))
model.add(Convolution2D(1024,3,3 ,border_mode='same', activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(Convolution2D(1024,3,3 ,border_mode='same', activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(Convolution2D(1024,3,3 ,border_mode='same', activation=LeakyReLU(alpha=0.1), trainable=not is_freeze))
model.add(Flatten())
model.add(Dense(256))
model.add(Dense(4096))
model.add(LeakyReLU(alpha=0.1))
model.add(Dense(1470))

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

convert_darkweights2keras(model, weights_path, verbalise=True)

model.summary()

