import numpy as np
import pickle
import os

def parser(model):
	"""
	Read the .cfg file to extract layers into `layers`
	as well as model-specific parameters into `meta`
	"""
	
	def _parse(l, i = 1):
		return l.split('=')[i].strip()

	with open(model, 'rb') as f:
		lines = f.readlines()

	lines = [line.decode() for line in lines]	
	
	meta = dict(); layers = list() # will contains layers' info
	h, w, c = [int()] * 3; layer = dict()
	for line in lines:
		line = line.strip()
		line = line.split('#')[0]
		if '[' in line:
			if layer != dict(): 
				if layer['type'] == '[net]': 
					h = layer['height']
					w = layer['width']
					c = layer['channels']
					meta['net'] = layer
				else:
					if layer['type'] == '[crop]':
						h = layer['crop_height']
						w = layer['crop_width']
					layers += [layer]				
			layer = {'type': line}
		else:
			try:
				i = float(_parse(line))
				if i == int(i): i = int(i)
				layer[line.split('=')[0].strip()] = i
			except:
				try:
					key = _parse(line, 0)
					val = _parse(line, 1)
					layer[key] = val
				except:
					'banana ninja yadayada'

	meta.update(layer) # last layer contains meta info
	if 'anchors' in meta:
		splits = meta['anchors'].split(',')
		anchors = [float(x.strip()) for x in splits]
		meta['anchors'] = anchors
	meta['model'] = model # path to cfg, not model name
	meta['inp_size'] = [h, w, c]
	return layers, meta