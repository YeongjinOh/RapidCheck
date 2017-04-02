from cfg.process import parser, cfg_yielder
from utils import loader
from dark.darkop import create_darkop
import warnings
import time
import os

class Darknet(object):
	_EXT = '.weights'

	def __init__(self, FLAGS):
		self.get_weight_src(FLAGS)
		self.modify = False

		print('Parsing {}'.format(self.src_cfg))
		src_parsed = self.parse_cfg(self.src_cfg, FLAGS)
		self.src_meta, self.src_layers = src_parsed

		if self.src_cfg == FLAGS.model:
			print("in self.src_cfg == FLAGS.model...")
			self.meta, self.layers = src_parsed
		else: 
			print('Parsing {}'.format(FLAGS.model))
			print("in self.src_cfg != FLAGS.model...")
			des_parsed = self.parse_cfg(FLAGS.model, FLAGS)
			self.meta, self.layers = des_parsed

		self.load_weights()
		print("self.load_weights() done...")
		print("[Darkent Init Done]..")

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
		layers, meta2 = parser(model)
		# print("Darkent::parse_cfg >> layers : ");print(layers)
		# print("layers 0 ## : ", layers[0])
		# print("layers 1 ## : ", layers[1])
		# print("layers 2 ## : ", layers[2])
		# print("layers len : ", len(layers))
		cfg_layers = cfg_yielder(*args)
		meta = dict(); layers = list()
		for i, info in enumerate(cfg_layers):
			# print("{} ## info : {} ".format(i,info))
			if i == 0: meta = info; continue
			else: new = create_darkop(*info)
			layers.append(new)
		# print("layers : "); print(layers) # 각각의 layer 에 대한 함수포인터값이 리턴된다.
		# print("meta : {}".format(len(meta))); print(meta)
		# print("meta2 : {}".format(len(meta2))); print(meta2)
		# print(meta2['out_size'])
		# print(meta2 == meta)

		return meta, layers
		# print("meta : ", meta)
		# print("Darkent::parse_cfg >> meta : ");print(meta)
		# print("meta anchors ## : ", meta['anchors'])
	
	def load_weights(self):
		"""
		Use `layers` and Loader to load .weights file
		"""
		print('Loading {} ...'.format(self.src_bin))
		start = time.time()

		args = [self.src_bin, self.src_layers]
		wgts_loader = loader.create_loader(*args)
		for layer in self.layers: layer.load(wgts_loader)

		stop = time.time()
		print('Finished in {}s'.format(stop - start))
		