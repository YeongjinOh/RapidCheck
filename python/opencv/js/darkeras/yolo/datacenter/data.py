from . pascal_voc_clean_xml import pascal_voc_clean_xml
from yolo.process import preprocess
from numpy.random import permutation as perm
from copy import deepcopy
import pickle
import numpy as np
import os
import yolo.config as cfg

def parse(exclusive = False):
	"""
	Decide whether to parse the annotation or not, 
	If the parsed file is not already there, parse.
	"""
	ext = '.parsed'
	history = os.path.join('yolo', 'parse-history.txt');
	if not os.path.isfile(history):
		file = open(history, 'w')
		file.close()
	with open(history, 'r') as f:
		lines = f.readlines()
	for line in lines:
		line = line.strip().split(' ')
		labels = line[1:]
		if labels == cfg.classes_name:
			if os.path.isfile(line[0]):
				with open(line[0], 'rb') as f:
					return pickle.load(f, encoding = 'latin1')[0]

	# actual parsing
	ann = cfg.ann_location
	if not os.path.isdir(ann):
		msg = 'Annotation directory not found {} .'
		exit('Error: {}'.format(msg.format(ann)))
	print('\n{} parsing {}'.format(cfg.model_name, ann))
	dumps = pascal_voc_clean_xml(ann, cfg.classes_name, exclusive)

	save_to = os.path.join('yolo', cfg.model_name)
	while True:
		if not os.path.isfile(save_to + ext): break
		save_to = save_to + '_'
	save_to += ext

	with open(save_to, 'wb') as f:
		pickle.dump([dumps], f, protocol = -1)
	with open(history, 'a') as f:
		f.write('{} '.format(save_to))
		f.write(' '.join(cfg.classes_name))
		f.write('\n')
	print('Result saved to {}'.format(save_to))
	return dumps

def _batch(chunk):
	"""
	Takes a chunk of parsed annotations
	returns value for placeholders of net's 
	input & loss layer correspond to this chunk
	chunk : ['006098.jpg', [375, 500, [['boat', 92, 74, 292, 178], ['bird', 239, 88, 276, 133], ['bird', 93, 100, 142, 140]]]]
	"""
	S, B = cfg.cell_size, cfg.boxes_per_cell
	C, labels = cfg.num_classes, cfg.classes_name

	# preprocess
	jpg = chunk[0]; w, h, allobj_ = chunk[1]
	allobj = deepcopy(allobj_)
	path = os.path.join(cfg.imageset_location, jpg)
	img = preprocess(path, allobj)

	# Calculate regression target
	cellx = 1. * w / S
	celly = 1. * h / S
	for obj in allobj:
		centerx = .5*(obj[1]+obj[3]) #xmin, xmax
		centery = .5*(obj[2]+obj[4]) #ymin, ymax
		cx = centerx / cellx
		cy = centery / celly
		if cx >= S or cy >= S: return None, None
		obj[3] = float(obj[3]-obj[1]) / w
		obj[4] = float(obj[4]-obj[2]) / h
		obj[3] = np.sqrt(obj[3])
		obj[4] = np.sqrt(obj[4])
		obj[1] = cx - np.floor(cx) # centerx
		obj[2] = cy - np.floor(cy) # centery
		obj += [int(np.floor(cy) * S + np.floor(cx))]

	# show(im, allobj, S, w, h, cellx, celly) # unit test

	# Calculate placeholders' values
	probs = np.zeros([S*S,C])
	confs = np.zeros([S*S,B])
	coord = np.zeros([S*S,B,4])
	proid = np.zeros([S*S,C])
	prear = np.zeros([S*S,4])
	for obj in allobj:
		# print(type(obj), obj) # <class 'list'> ['horse', 0.024000000000000021, 0.48952095808383245, 0.92303846073714613, 0.85995404416970578, 24]
		probs[obj[5], :] = [0.] * C
		probs[obj[5], labels.index(obj[0])] = 1.
		proid[obj[5], :] = [1] * C
		coord[obj[5], :, :] = [obj[1:5]] * B
		prear[obj[5],0] = obj[1] - obj[3]**2 * .5 * S # xleft
		prear[obj[5],1] = obj[2] - obj[4]**2 * .5 * S # yup
		prear[obj[5],2] = obj[1] + obj[3]**2 * .5 * S # xright
		prear[obj[5],3] = obj[2] + obj[4]**2 * .5 * S # ybot
		confs[obj[5], :] = [1.] * B

	# Finalise the placeholders' values
	upleft   = np.expand_dims(prear[:,0:2], 1)
	botright = np.expand_dims(prear[:,2:4], 1)
	wh = botright - upleft; 
	area = wh[:,:,0] * wh[:,:,1]
	upleft   = np.concatenate([upleft] * B, 1)
	botright = np.concatenate([botright] * B, 1)
	areas = np.concatenate([area] * B, 1)

	# value for placeholder at input layer
	inp_feed_val = img
	# value for placeholder at loss layer 
	loss_feed_val = {
		'probs': probs, 'confs': confs, 
		'coord': coord, 'proid': proid,
		'areas': areas, 'upleft': upleft, 
		'botright': botright
	}

	return inp_feed_val, loss_feed_val

def shuffle():
	batch_size = cfg.batch_size
	data = parse()
	size = len(data)
	print('Dataset of {} instance(s)'.format(size))
	if batch_size > size: 
		# 전체데이터가 Batch Size 보다 적을때를 대비하여
		cfg.batch_size = batch_size = size
	batch_per_epoch = int(size/batch_size)

	for i in range(cfg.epochs):
		shuffle_idx = perm(np.arange(size))
		# print("shuffle index : ", shuffle_idx)
		for b in range(batch_per_epoch):
			# yield these
			x_batch = list()
			feed_batch = dict()

			for j in range(b*batch_size, b*batch_size + batch_size):
				train_instance = data[shuffle_idx[j]]
				inp, new_feed = _batch(train_instance)

				if inp is None: continue
				x_batch += [np.expand_dims(inp, 0)] # inp.shape : 448, 448, 3
				for key in new_feed:
					new = new_feed[key]
					old_feed = feed_batch.get(key, np.zeros((0,)+new.shape))
					feed_batch[key] = np.concatenate([old_feed, [new]])
			# print("feed_batch : ", len(feed_batch), feed_batch['botright'].shape) # feed_batch :  7 (32, 49, 2, 2)
			# print("x_batch[0].shape : ", x_batch[0].shape) # x_batch.shape :  (1, 448, 448, 3)
			
			x_batch = np.concatenate(x_batch, 0)
			yield x_batch, feed_batch

		print('Finish {} epoch'.format(i+1))

if __name__ == '__main__':
	print("hello")