import keras
from keras.models import Sequential
from keras.layers.convolutional import Conv2D, MaxPooling2D
from keras.layers.advanced_activations import LeakyReLU
from keras.layers.core import Flatten, Dense, Activation, Reshape
from keras.layers import Dropout
import yolo.config as cfg

is_freeze = True
output_tensor_shape = (cfg.cell_size * cfg.cell_size)*(cfg.num_classes + cfg.boxes_per_cell*5)
inp_w, inp_h, inp_c = cfg.inp_size

def yolo_tiny_THdim_model(is_freeze=is_freeze):
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
	model.add(Dense(4096, name='new_dense2', activation=LeakyReLU(alpha=0.1)))
	# model.add(LeakyReLU(alpha=0.1, name='leaky1'))
	model.add(Dense(output_tensor_shape, name='new_detection'))

	return model

def yolo_tiny_THdim_dropout_model(is_freeze=is_freeze):
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
	model.add(Dense(4096, name='new_dense2', activation=LeakyReLU(alpha=0.1)))
	model.add(Dropout(0.5))
	# model.add(LeakyReLU(alpha=0.1, name='leaky1'))
	model.add(Dense(output_tensor_shape, name='new_detection'))

	return model

def yolo_shortdense_THdim_model(is_freeze=is_freeze):
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
	model.add(Dense(512, name='new_dense1'))
	model.add(LeakyReLU(alpha=0.1))
	model.add(Dense(output_tensor_shape, name='new_detection'))

	return model