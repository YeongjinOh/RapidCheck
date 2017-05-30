import numpy as np
import tensorflow as tf
import tensorflow.contrib.slim as slim
import yolo.config as cfg

_TRAINER = dict({
		'rmsprop': tf.train.RMSPropOptimizer,
		'adadelta': tf.train.AdadeltaOptimizer,
		'adagrad': tf.train.AdagradOptimizer,
		'adagradDA': tf.train.AdagradDAOptimizer,
		'momentum': tf.train.MomentumOptimizer,
		'adam': tf.train.AdamOptimizer,
		'ftrl': tf.train.FtrlOptimizer,
	})

# def _to_tensor(x, dtype):
# 	x = tf.convert_to_tensor(x)
# 	if x.dtype != dtype:
# 		x = tf.cast(x, dtype)
# 	return x

def darkeras_loss(net_out):
	
	sprob = float(cfg.class_scale)
	sconf = float(cfg.object_scale)
	snoob = float(cfg.noobject_scale)
	scoor = float(cfg.coord_scale)
	S, B, C = cfg.cell_size, cfg.boxes_per_cell, cfg.num_classes
	SS = S * S # number of grid cells

	size1 = [None, SS, C]
	size2 = [None, SS, B]

	# return the below placeholders
	_probs = tf.placeholder(tf.float32, size1)
	_confs = tf.placeholder(tf.float32, size2)
	_coord = tf.placeholder(tf.float32, size2 + [4])
	# weights term for L2 loss
	_proid = tf.placeholder(tf.float32, size1)
	# material calculating IOU
	_areas = tf.placeholder(tf.float32, size2)
	_upleft = tf.placeholder(tf.float32, size2 + [2])
	_botright = tf.placeholder(tf.float32, size2 + [2])

	placeholders = {
		'probs':_probs, 'confs':_confs, 'coord':_coord, 'proid':_proid,
		'areas':_areas, 'upleft':_upleft, 'botright':_botright
	}

	
	# Extract the coordinate prediction from net.out
	coords = net_out[:, SS * (C + B):]
	coords = tf.reshape(coords, [-1, SS, B, 4])
	wh = tf.pow(coords[:,:,:,2:4], 2) * S # unit: grid cell
	area_pred = wh[:,:,:,0] * wh[:,:,:,1] # unit: grid cell^2
	centers = coords[:,:,:,0:2] # [batch, SS, B, 2]
	floor = centers - (wh * .5) # [batch, SS, B, 2]
	ceil  = centers + (wh * .5) # [batch, SS, B, 2]

	# calculate the intersection areas
	intersect_upleft   = tf.maximum(floor, _upleft)
	intersect_botright = tf.minimum(ceil , _botright)
	intersect_wh = intersect_botright - intersect_upleft
	intersect_wh = tf.maximum(intersect_wh, 0.0)
	intersect = tf.multiply(intersect_wh[:,:,:,0], intersect_wh[:,:,:,1])

	# calculate the best IOU, set 0.0 confidence for worse boxes
	iou = tf.truediv(intersect, _areas + area_pred - intersect)
	best_box = tf.equal(iou, tf.reduce_max(iou, [2], True))
	best_box = tf.to_float(best_box)
	confs = tf.multiply(best_box, _confs)

	# take care of the weight terms
	conid = snoob * (1. - confs) + sconf * confs
	weight_coo = tf.concat(4 * [tf.expand_dims(confs, -1)], 3)
	cooid = scoor * weight_coo
	proid = sprob * _proid

	# flatten 'em all
	probs = slim.flatten(_probs)
	proid = slim.flatten(proid)
	confs = slim.flatten(confs)
	conid = slim.flatten(conid)
	coord = slim.flatten(_coord)
	cooid = slim.flatten(cooid)

	# reshape 1 dim vevtor
	# probs = tf.reshape(_probs, [-1])
	# proid = tf.reshape(proid, [-1])
	# confs = tf.reshape(confs, [-1])
	# conid = tf.reshape(conid, [-1])
	# coord = tf.reshape(_coord, [-1])
	# cooid = tf.reshape(cooid, [-1])

	true = tf.concat([probs, confs, coord], 1)
	wght = tf.concat([proid, conid, cooid], 1)

	print('Building {} loss'.format(cfg.model_name))
	loss = tf.pow(net_out - true, 2)
	loss = tf.multiply(loss, wght)
	loss = tf.reduce_sum(loss, 1)
	return placeholders, .5 * tf.reduce_mean(loss)


if __name__ == '__main__':
	print(cfg.cell_size)