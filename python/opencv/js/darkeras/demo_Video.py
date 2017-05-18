
import os
import glob
import h5py
import numpy as np
import cv2
import matplotlib.pyplot as plt
from keras import backend as K

import tensorflow as tf
import yolo.config as cfg

from yolo.net.yolo_tiny_thdim_net import yolo_tiny_THdim_model
from utils.BoxUtils import post_progress
from yolo.process import preprocess

from utils.help import DB_Helper, DB_Item

K.set_image_dim_ordering('th')

is_freeze = True
# weigths_path = 'models/train/yolo-2class-epoch50.h5'
weigths_path = 'models/train/yolo-2class-complete.h5'
test_threshold = 0.24

model = yolo_tiny_THdim_model(is_freeze)
model.load_weights(weigths_path)
model.summary()

db = DB_Helper()
db.open()
db.delete()
# video_name = 'persons1.mp4'
# video_name = 'tracking.mp4'
video_name = 'demo2.mp4'
# video_name = 'playback.mp4'
videoId = 3 # TODO: video id would be get by runtime
frameNum = 0
frameSteps = 5
items = []
cap = cv2.VideoCapture('C:\\Users\\SoMa\\myworkspace\\RapidCheck\\python\\opencv\\js\\darkeras\\test\\my_testset\\'+video_name)
while True:
	ret, frame = cap.read()
	frameNum += 1
	if frameNum % frameSteps != 0:
		continue
	# cv2.imshow('Original Window', frame)
	img = preprocess(frame)
	batch = np.expand_dims(img, axis=0)
	net_out = model.predict(batch)
	out_img, class_index, x, y, w, h, is_object = post_progress(net_out[0], im=frame, is_save=False, threshold=test_threshold)
	if is_object:
		items.append(DB_Item(videoId, frameNum, class_index, x, y, w, h))
	
	if len(items) == 100:
		db.insert(items)
		print("DB Insert 100 items Done..")
		del items
		items = []
	cv2.imshow('Detection Window', out_img)
	if cv2.waitKey(1) & 0xFF == ord('q'):
		break

cap.release()
cv2.destroyAllWindows()

db.close()