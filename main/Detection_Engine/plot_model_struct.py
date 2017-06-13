from keras.utils import plot_model
import h5py
from keras import backend as K

import tensorflow as tf
import yolo.config as cfg

from yolo.net.yolo_tiny_thdim_net import yolo_tiny_THdim_model, yolo_shortdense_THdim_model

if __name__ == '__main__':
	K.set_image_dim_ordering('th')

	is_freeze = True
	model = yolo_tiny_THdim_model(is_freeze)
	
	plot_model(model, to_file='model.png', show_shapes=True, show_layer_names=True)