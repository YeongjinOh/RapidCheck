import tensorflow as tf
import time
from dark.darknet import Darknet
from .framework import create_framework
from . import flow
from . import help
from .ops import op_create, identity, HEADER, LINE

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
	say = help.say

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

		self.meta = darknet.meta
		print("self.meta : ", self.meta)
		self.FLAGS = FLAGS
		self.say("This is FLAGS : ", self.FLAGS)

		self.say('\nBuilding net ...')
		start = time.time()
		self.graph = tf.Graph()

		with self.graph.as_default() as g:
			self.build_forward()
			self.setup_meta_ops()
		self.say('Finished in {}s\n'.format(
			time.time() - start))

	def build_forward(self):
		
		# Placeholders
		inp_size = [None] + self.meta['inp_size']
		print("build_forward::inp_size.shape : ", inp_size)
		self.inp = tf.placeholder(tf.float32, inp_size, 'input')
		self.feed = dict() # other placeholders

		# Build the forward pass
		state = identity(self.inp)
		roof = self.num_layer - self.ntrain
		self.say("roof : ", roof)
		self.say(HEADER, LINE)


	def setup_meta_ops(self):
		pass
