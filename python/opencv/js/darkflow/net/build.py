import tensorflow as tf
import time
from dark.darknet import Darknet
from .framework import create_framework
from . import flow

class dotdict(dict):
	"""dot.notation access to dictionary attributes to replace FLAGS when not needed"""
	__getattr__ = dict.get
	__setattr__ = dict.__setitem__
	__delattr__ = dict.__delitem__


class TFNet(object):
	_TRAINER = dict({
		'rmsprop': tf.train.RMSPropOptimizer,
		'adadelta': tf.train.AdadeltaOptimizer,
		'adagrad': tf.train.AdagradOptimizer,
		'adagradDA': tf.train.AdagradDAOptimizer,
		'momentum': tf.train.MomentumOptimizer,
		'adam': tf.train.AdamOptimizer,
		'ftrl': tf.train.FtrlOptimizer,
	})

	# import methods
	return_predict = flow.return_predict

	def __init__(self, FLAGS, darkent=None):
		self.ntrain = 0
		if isinstance(FLAGS, dict):
			defaultSettings = {"binary":"./bin/", "config":"./cfg/", 
							"batch":16, "threshold":0.1,
							"train":False, "verbalise":False, "gpu":0.0}
			defaultSettings.update(FLAGS)
			FLAGS = dotdict(defaultSettings)

		if darkent is None:
			print("Initialize Darkent..")
			darknet = Darknet(FLAGS)
			self.ntrain = len(darknet.layers)
			print("self.ntrain : {}".format(self.ntrain))

		self.darknet = darknet
		args = [darknet.meta, FLAGS]
		self.num_layer = len(darknet.layers)
		print("self.num_layer : {}".format(self.num_layer))
		self.framework = create_framework(*args)

		print("This is FLAGS : ", FLAGS)