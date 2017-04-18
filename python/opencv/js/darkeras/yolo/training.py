import numpy as np
import tensorflow as tf
import config as cfg

def iou(boxes1, boxes2):
	"""calculate ious
	Args:
	  boxes1: 4-D tensor [CELL_SIZE, CELL_SIZE, BOXES_PER_CELL, 4]  ====> (x_center, y_center, w, h)
	  boxes2: 1-D tensor [4] ===> (x_center, y_center, w, h)
	Return:
	  iou: 3-D tensor [CELL_SIZE, CELL_SIZE, BOXES_PER_CELL]
	"""
	boxes1 = tf.stack([boxes1[:, :, :, 0] - boxes1[:, :, :, 2] / 2, boxes1[:, :, :, 1] - boxes1[:, :, :, 3] / 2,
					  boxes1[:, :, :, 0] + boxes1[:, :, :, 2] / 2, boxes1[:, :, :, 1] + boxes1[:, :, :, 3] / 2])
	boxes1 = tf.transpose(boxes1, [1, 2, 3, 0])
	boxes2 =  tf.stack([boxes2[0] - boxes2[2] / 2, boxes2[1] - boxes2[3] / 2,
					  boxes2[0] + boxes2[2] / 2, boxes2[1] + boxes2[3] / 2])

	#calculate the left up point
	lu = tf.maximum(boxes1[:, :, :, 0:2], boxes2[0:2])
	rd = tf.minimum(boxes1[:, :, :, 2:], boxes2[2:])

	#intersection
	intersection = rd - lu 

	inter_square = intersection[:, :, :, 0] * intersection[:, :, :, 1]

	mask = tf.cast(intersection[:, :, :, 0] > 0, tf.float32) * tf.cast(intersection[:, :, :, 1] > 0, tf.float32)
	
	inter_square = mask * inter_square
	
	#calculate the boxs1 square and boxs2 square
	square1 = (boxes1[:, :, :, 2] - boxes1[:, :, :, 0]) * (boxes1[:, :, :, 3] - boxes1[:, :, :, 1])
	square2 = (boxes2[2] - boxes2[0]) * (boxes2[3] - boxes2[1])
	
	return inter_square/(square1 + square2 - inter_square + 1e-6)

def cond1(num, object_num, loss, predict, label, nilboy):
	"""
	if num < object_num
	"""
	return num < object_num

def body1(num, object_num, loss, predict, labels, nilboy):
	"""
	calculate loss
	Args:
	  predict: 3-D tensor [cell_size, cell_size, 5 * boxes_per_cell]
	  labels : [max_objects, 5]  (x_center, y_center, w, h, class)
	"""
	label = labels[num:num+1, :]
	label = tf.reshape(label, [-1])

	#calculate objects  tensor [CELL_SIZE, CELL_SIZE]
	#calculate responsible tensor [CELL_SIZE, CELL_SIZE]
	center_x = label[0] / (cfg.image_size / cfg.cell_size)
	center_x = tf.floor(center_x)

	center_y = label[1] / (cfg.image_size / cfg.cell_size)
	center_y = tf.floor(center_y)

	response = tf.ones([1, 1], tf.float32)

	temp = tf.cast(tf.stack([center_y, cfg.cell_size - center_y - 1, center_x, cfg.cell_size -center_x - 1]), tf.int32)
	temp = tf.reshape(temp, (2, 2))
	response = tf.pad(response, temp, "CONSTANT")
	objects = response

	#calculate iou_predict_truth [CELL_SIZE, CELL_SIZE, BOXES_PER_CELL]
	predict_boxes = predict[:, :, cfg.num_classes + cfg.boxes_per_cell:]
	

	predict_boxes = tf.reshape(predict_boxes, [cfg.cell_size, cfg.cell_size, cfg.boxes_per_cell, 4])

	predict_boxes = predict_boxes * [cfg.image_size / cfg.cell_size, cfg.image_size / cfg.cell_size, cfg.image_size, cfg.image_size]

	base_boxes = np.zeros([cfg.cell_size, cfg.cell_size, 4])

	for y in range(cfg.cell_size):
		for x in range(cfg.cell_size):
			#nilboy
			base_boxes[y, x, :] = [cfg.image_size / cfg.cell_size * x, cfg.image_size / cfg.cell_size * y, 0, 0]
	base_boxes = np.tile(np.resize(base_boxes, [cfg.cell_size, cfg.cell_size, 1, 4]), [1, 1, cfg.boxes_per_cell, 1])

	predict_boxes = base_boxes + predict_boxes

	iou_predict_truth = cfg.iou(predict_boxes, label[0:4])
	#calculate C [cell_size, cell_size, boxes_per_cell]
	C = iou_predict_truth * tf.reshape(objects, [cfg.cell_size, cfg.cell_size, 1])

	#calculate I tensor [CELL_SIZE, CELL_SIZE, BOXES_PER_CELL]
	I = iou_predict_truth * tf.reshape(response, (cfg.cell_size, cfg.cell_size, 1))
	
	max_I = tf.reduce_max(I, 2, keep_dims=True)

	I = tf.cast((I >= max_I), tf.float32) * tf.reshape(response, (cfg.cell_size, cfg.cell_size, 1))

	#calculate no_I tensor [CELL_SIZE, CELL_SIZE, BOXES_PER_CELL]
	no_I = tf.ones_like(I, dtype=tf.float32) - I 


	p_C = predict[:, :, cfg.num_classes:cfg.num_classes + cfg.boxes_per_cell]

	#calculate truth x,y,sqrt_w,sqrt_h 0-D
	x = label[0]
	y = label[1]

	sqrt_w = tf.sqrt(tf.abs(label[2]))
	sqrt_h = tf.sqrt(tf.abs(label[3]))
	#sqrt_w = tf.abs(label[2])
	#sqrt_h = tf.abs(label[3])

	#calculate predict p_x, p_y, p_sqrt_w, p_sqrt_h 3-D [CELL_SIZE, CELL_SIZE, BOXES_PER_CELL]
	p_x = predict_boxes[:, :, :, 0]
	p_y = predict_boxes[:, :, :, 1]

	#p_sqrt_w = tf.sqrt(tf.abs(predict_boxes[:, :, :, 2])) * ((tf.cast(predict_boxes[:, :, :, 2] > 0, tf.float32) * 2) - 1)
	#p_sqrt_h = tf.sqrt(tf.abs(predict_boxes[:, :, :, 3])) * ((tf.cast(predict_boxes[:, :, :, 3] > 0, tf.float32) * 2) - 1)
	#p_sqrt_w = tf.sqrt(tf.maximum(0.0, predict_boxes[:, :, :, 2]))
	#p_sqrt_h = tf.sqrt(tf.maximum(0.0, predict_boxes[:, :, :, 3]))
	#p_sqrt_w = predict_boxes[:, :, :, 2]
	#p_sqrt_h = predict_boxes[:, :, :, 3]
	p_sqrt_w = tf.sqrt(tf.minimum(cfg.image_size * 1.0, tf.maximum(0.0, predict_boxes[:, :, :, 2])))
	p_sqrt_h = tf.sqrt(tf.minimum(cfg.image_size * 1.0, tf.maximum(0.0, predict_boxes[:, :, :, 3])))
	#calculate truth p 1-D tensor [NUM_CLASSES]
	P = tf.one_hot(tf.cast(label[4], tf.int32), cfg.num_classes, dtype=tf.float32)

	#calculate predict p_P 3-D tensor [CELL_SIZE, CELL_SIZE, NUM_CLASSES]
	p_P = predict[:, :, 0:cfg.num_classes]

	#class_loss
	class_loss = tf.nn.l2_loss(tf.reshape(objects, (cfg.cell_size, cfg.cell_size, 1)) * (p_P - P)) * cfg.class_scale
	#class_loss = tf.nn.l2_loss(tf.reshape(response, (cfg.cell_size, cfg.cell_size, 1)) * (p_P - P)) * cfg.class_scale

	#object_loss
	object_loss = tf.nn.l2_loss(I * (p_C - C)) * cfg.object_scale
	#object_loss = tf.nn.l2_loss(I * (p_C - (C + 1.0)/2.0)) * cfg.object_scale

	#noobject_loss
	#noobject_loss = tf.nn.l2_loss(no_I * (p_C - C)) * cfg.noobject_scale
	noobject_loss = tf.nn.l2_loss(no_I * (p_C)) * cfg.noobject_scale

	#coord_loss
	coord_loss = (tf.nn.l2_loss(I * (p_x - x)/(cfg.image_size/cfg.cell_size)) +
				 tf.nn.l2_loss(I * (p_y - y)/(cfg.image_size/cfg.cell_size)) +
				 tf.nn.l2_loss(I * (p_sqrt_w - sqrt_w))/ cfg.image_size +
				 tf.nn.l2_loss(I * (p_sqrt_h - sqrt_h))/cfg.image_size) * cfg.coord_scale

	nilboy = I

	return num + 1, object_num, [loss[0] + class_loss, loss[1] + object_loss, loss[2] + noobject_loss, loss[3] + coord_loss], predict, labels, nilboy

def darkeras_loss(y_true, y_pred):
	n1 = cfg.cell_size * cfg.cell_size * cfg.num_classes
	n2 = n1 + cfg.cell_size * cfg.cell_size * cfg.boxes_per_cell

	class_probs = tf.reshape(local3[:, 0:n1], (-1, cfg.cell_size, cfg.cell_size, cfg.num_classes))
	scales = tf.reshape(local3[:, n1:n2], (-1, cfg.cell_size, cfg.cell_size, cfg.boxes_per_cell))
	boxes = tf.reshape(local3[:, n2:], (-1, cfg.cell_size, cfg.cell_size, cfg.boxes_per_cell * 4))

	y_pred = tf.concat([class_probs, scales, boxes], 3)
	pass

if __name__ == '__main__':
	print(cfg.cell_size)