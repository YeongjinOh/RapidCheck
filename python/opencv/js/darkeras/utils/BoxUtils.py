import numpy as np
import cv2

classes_name =  ["aeroplane", "bicycle", "bird", "boat", "bottle", "bus", "car", "cat", "chair", "cow", "diningtable", "dog", "horse", "motorbike", "person", "pottedplant", "sheep", "sofa", "train","tvmonitor"]
base = int(np.ceil(pow(len(classes_name), 1./3)))
print("base : ", base)
def _to_color(index, base):
	""" return (b, r, g) tuple"""
	base2 = base * base
	b = 2 - index / base2
	r = 2 - (index % base2) / base
	g = 2 - (index % base2) % base
	return (b * 127, r * 127, g * 127)

classes_colors = []
for c_idx, c_name in enumerate(classes_name):
	classes_colors.append((c_name, _to_color(c_idx, base)))
	print(classes_colors[c_idx])


class Box:
	def __init__(self, classes):
		self.x, self.y, self.w, self.h, self.c = float(), float(), float(), float(), float()
		self.probs = np.zeros((classes,))
		self.class_num = classes

	def __repr__(self):
		return "x : {0:.2f}, y : {0:.2f}, w : {0:.2f}, h : {0:.2f}, confidence : {0:.2f}".format(
			self.x, self.y, self.w, self.h, self.c)

	def get_probs(self):
		return self.probs


def prob_compare(box):
	return box.probs[box.class_num]


def find_boxes(net_out, threshold = 0.01, sqrt=2,C=20, B=2, S=7):
	boxes = []
	SS        =  S * S # number of grid cells
	prob_size = SS * C # class probabilities
	conf_size = SS * B # confidences for each grid cell

	probs = net_out[0, :, :, 0:20]
	print("probs : ", probs.shape)
	probs = probs.reshape([SS, C])
	print("probs : ", probs.shape)

	confs = net_out[0, :, :, 20:22]
	print("confs : ", confs.shape)
	confs = confs.reshape([SS, B])
	print("confs : ", confs.shape)


	cords = net_out[0, :, :, 22:]
	print("cords : ", cords.shape)
	cords = cords.reshape([SS, B, 4])
	print("cords : ", cords.shape)

	for grid in range(SS):
		for b in range(B):
			bx   = Box(C)
			bx.c =  confs[grid, b]
			bx.x = (cords[grid, b, 0] + grid %  S) / S
			bx.y = (cords[grid, b, 1] + grid // S) / S
			bx.w =  cords[grid, b, 2]** sqrt
			if np.isnan(bx.w):
				print(cords[grid, b,2], cords[grid, b, 2] ** 1.8)
				print("----------")
				exit(0) 
			bx.h =  cords[grid, b, 3]** sqrt
			# print("probs[{},:] : ".format(grid), probs[grid, :])
			p = probs[grid, :] * bx.c
			# print("p : ", p.shape, p)
			p *= (p > threshold)
			bx.probs = p
			print(bx.probs)
			boxes.append(bx)

	for c in range(C):
		for i in range(len(boxes)):
			boxes[i].class_num = c
		boxes = sorted(boxes, key=prob_compare, reverse=True)
		for i in range(len(boxes)):
			boxi = boxes[i]
			if boxi.probs[c] == 0: continue
			for j in range(i + 1, len(boxes)):
				boxj = boxes[j]
			if box_iou(boxi, boxj) >= .4:
				boxes[j].probs[c] = 0.
	return boxes

def overlap(x1,w1,x2,w2):
    l1 = x1 - w1 / 2.;
    l2 = x2 - w2 / 2.;
    left = max(l1, l2)
    r1 = x1 + w1 / 2.;
    r2 = x2 + w2 / 2.;
    right = min(r1, r2)
    return right - left;

def box_intersection(a, b):
    w = overlap(a.x, a.w, b.x, b.w);
    h = overlap(a.y, a.h, b.y, b.h);
    if w < 0 or h < 0: return 0;
    area = w * h;
    return area;

def box_union(a, b):
    i = box_intersection(a, b);
    u = a.w * a.h + b.w * b.h - i;
    return u;

def box_iou(a, b):
    return box_intersection(a, b) / box_union(a, b);

def process_box(b, h, w, threshold):
	max_indx = np.argmax(b.probs)
	max_prob = b.probs[max_indx]

	if max_prob > threshold:
		print(b.x)
		print(b.w)
		print(w)
		left  = int ((b.x - b.w/2.) * w)
		right = int ((b.x + b.w/2.) * w)
		top   = int ((b.y - b.h/2.) * h)
		bot   = int ((b.y + b.h/2.) * h)
		if left  < 0    :  left = 0
		if right > w - 1: right = w - 1
		if top   < 0    :   top = 0
		if bot   > h - 1:   bot = h - 1
		return (left, right, top, bot, max_indx, max_prob)
	return None

def post_progress(net_out, im, is_save = True, threshold=0.01):
	boxes = find_boxes(net_out, threshold=threshold)

	if type(im) is not np.ndarray:
		imgcv = cv2.imread(im)
	else: imgcv = im

	h, w, _ = imgcv.shape
	thick = int((h + w) // 300) # ractange line thick
	for b in boxes:
		boxResults = process_box(b, h, w, threshold)
		if boxResults is None:
	  		continue
		left, right, top, bot, max_indx, confidence = boxResults
		line = "max_index: {}, left: {}, top: {}, right: {}, bottom: {}".format(
	    	max_indx, left, top, right, bot)
		print(line)

		class_name, class_color = classes_colors[max_indx]
		cv2.rectangle(imgcv,
	  		(left, top), (right, bot),
	  		class_color, thick)
	# cv2.putText(imgcv, max_indx, (left, top - 12),
	#    2, 1.5, (0, 0, 255))
	cv2.putText(imgcv, class_name, (int(left), int(top-12)), 2, 1.5, class_color)
	if not is_save: 
		return imgcv
	else:
		cv2.imwrite('test\out.jpg', imgcv)