import os
import time
import numpy as np
import tensorflow as tf

def return_predict(self, im):
	assert isinstance(im, np.ndarray), \
				'Image is not a np.ndarray'
	h, w, _ = im.shape