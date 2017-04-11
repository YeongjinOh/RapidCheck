from cfg.process import parser
from utils import loader
import warnings
import time
import os

class Darknet(object):
	_EXT = '.weights'

	def __init__(self, FLAGS):
		self.get_weight_src(FLAGS)
		self.modify = False
		print('Parsing {}'.format(self.src_cfg))
		self.parse_cfg(self.src_cfg, FLAGS)


	def get_weight_src(self, FLAGS):
		"""
		analyse FLAGS.load to know where is the 
		source binary and what is its config.
		can be: None, FLAGS.model, or some other
		"""
		self.src_bin = FLAGS.binary + FLAGS.model + self._EXT
		self.src_bin = os.path.abspath(self.src_bin)
		exist = os.path.isfile(self.src_bin)

		if FLAGS.load == str(): FLAGS.load = int()
		if type(FLAGS.load) is int:
			self.src_cfg = FLAGS.model
			print(self.src_cfg)
		if FLAGS.load: 
			self.src_bin = None
			print("FLAGS.load is True", FLAGS.load)
		elif not exist: 
			self.src_bin = None
			print("not exist")
		else:
			assert os.path.isfile(FLAGS.load), \
				'{} not found'.format(FLAGS.load)
			self.src_bin = FLAGS.load
			name = loader.model_name(FLAGS.load)
			cfg_path = os.path.join(FLAGS.config, name + '.cfg')
			if not os.path.isfile(cfg_path):
				warnings.warn('{} not found, use {} instead'.format(cfg_path, FLAGS.model))
				cfg_path = FLAGS.model
			self.src_cfg = cfg_path
			FLAGS.load = int()

	def parse_cfg(self, model, FLAGS):
		"""
		return a list of `layers` objects (darkop.py)
		given path to binaries/ and configs/
		"""
		args = [model, FLAGS.binary]
		layers, meta = parser(model)
		# print("Darkent::parse_cfg >> layer : ");print(layer)
		# print("layers 0 ## : ", layers[0])
		# print("layers 1 ## : ", layers[1])
		# print("layers 2 ## : ", layers[2])
		# print("layers len : ", len(layer))
		print("Darkent::parse_cfg >> meta : ");print(meta)
		print("meta anchors ## : ", meta['anchors'])
		