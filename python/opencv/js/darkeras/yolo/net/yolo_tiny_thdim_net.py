import keras
from keras.models import Sequential
from keras.layers.convolutional import Conv2D, MaxPooling2D
from keras.layers.advanced_activations import LeakyReLU
from keras.layers.core import Flatten, Dense, Activation, Reshape
import yolo.config as cfg

is_freeze = True
output_tensor_shape = (cfg.cell_size * cfg.cell_size)*(cfg.num_classes + cfg.boxes_per_cell*5)


def yolo_tiny_THNet(is_freeze=is_freeze):
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
	model.add(Dense(output_tensor_shape))

	return model