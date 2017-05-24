
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
test_threshold = 0.23

model = yolo_tiny_THdim_model(is_freeze)
model.load_weights(weigths_path)
model.summary()

db = DB_Helper()
db.open()
# db.delete()
# video_name = 'persons1.mp4'
video_name = 'tracking.mp4'
# video_name = 'demo2.mp4'
# video_name = 'videoplayback.mp4'
# video_name = 'car_video5.mp4'
# video_name = 'car_night_video.mp4'
# video_name  = 'cctv4.mp4'
# video_name = 'car_view_video3.avi'
videoId = 3 # TODO: video id would be get by runtime
frameNum = 0
frameSteps = 5
items = []
cap = cv2.VideoCapture('C:\\Users\\SoMa\\myworkspace\\RapidCheck\\python\\opencv\\js\\darkeras\\test\\my_testset\\'+video_name)
cv2.namedWindow('Detection Window',cv2.WINDOW_NORMAL)
cv2.resizeWindow('Detection Window', 600,600)
try:
	while True:
		ret, frame = cap.read()
		frameNum += 1
		if frameNum % frameSteps != 0:
			continue
		# cv2.imshow('Original Window', frame)
		print("FrameNum : {}".format(frameNum))
		img = preprocess(frame)
		batch = np.expand_dims(img, axis=0)
		net_out = model.predict(batch)
		out_img, objects, is_object = post_progress(net_out[0], im=frame, is_save=False, threshold=test_threshold)
		# if is_object:
		# 	for each_object in objects:
		# 		items.append(DB_Item(videoId, frameNum, each_object[0], each_object[1], each_object[2], each_object[3], each_object[4]))
		
		# if len(items) >= 100:
		# 	db.insert(items)
		# 	print("DB Insert 100 items Done..")
		# 	del items
		# 	items = []
		cv2.imshow('Detection Window', out_img)
		if cv2.waitKey(1) & 0xFF == ord('q'):
			break
except Exception:
	print("Exception Occured")
finally:
	db.close()
	print("DB Closed in Finally..")
	cap.release()
	cv2.destroyAllWindows()

# db.insert(items)
# print("DB Insert 100 items Done..")
# del items
