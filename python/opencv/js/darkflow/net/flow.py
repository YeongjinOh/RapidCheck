import os
import time
import numpy as np
import tensorflow as tf

def return_predict(self, im):
	assert isinstance(im, np.ndarray), \
				'Image is not a np.ndarray'
	h, w, _ = im.shape
	im = self.framework.resize_input(im)
	print("im shape : ", im.shape)
	this_inp = np.expand_dims(im, 0)
	print("this_inp shape : ", this_inp.shape)