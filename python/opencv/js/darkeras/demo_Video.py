
import os
import glob
import h5py
import numpy as np
import cv2
import matplotlib.pyplot as plt
from keras import backend as K

import tensorflow as tf
import yolo.config as cfg

from yolo.net.yolo_tiny_thdim_net import yolo_tiny_THdim_model, yolo_shortdense_THdim_model
from utils.BoxUtils import post_progress
from yolo.process import preprocess

from utils.help import DB_Helper, DB_Item
import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--videoId', type=int, help='VideoId for detecting works', required=True)
args = parser.parse_args()
# db.delete()
if args.videoId:
	videoId = args.videoId
	print("video id : ", args.videoId)
	# TODO : query video path from mysql
	db = DB_Helper()
	db.open(table_name='file')
	row = db.select(table_name='file', condition={'videoId':videoId})
	video_path = row['videoPath']
	frameSteps = row['frameSteps']
exit()
K.set_image_dim_ordering('th')

is_freeze = True
# weigths_path = 'models/train/yolo-2class-complete.h5'
weigths_path = os.path.join(cfg.model_folder, cfg.model_name) + '-steps8000.h5'
# weigths_path = 'models/train/'+cfg.model_name+'-complete.h5'
# weigths_path = 'models/train/yolo-2class-mydata-3video-steps5000.h5'
test_threshold = 0.4

# weigths_path = 'models/train/yolo-2class-voc2007-train-cell28-steps40000.h5'
# weigths_path = 'models/train/yolo-2class-mydata-3video-steps5000.h5'
model = yolo_tiny_THdim_model(is_freeze)
# model = yolo_shortdense_THdim_model(is_freeze)
model.load_weights(weigths_path)
model.summary()

# for compare 2 models
model2 = yolo_tiny_THdim_model(is_freeze)
model2.load_weights('models/train/yolo-2class-voc2007-train-cell14-steps40000.h5')

# video_name = 'persons1.mp4'
#video_name = 'apart_car1.mp4'
# video_name = 'demo2.mp4'
video_name = 'tracking.mp4'

# video_name = 'videoplayback.mp4'
# video_name = 'car_video2.mp4'
# video_name = 'car_night_video.mp4'
# video_name  = 'cctv4.mp4'
# video_name = 'car_view_video3.avi'

frameNum = 0
items = []
cap = cv2.VideoCapture(video_path)
# cap = cv2.VideoCapture('C:\\Users\\SoMa\\myworkspace\\RapidCheck\\python\\opencv\\js\\darkeras\\test\\my_testset\\'+video_name)
# cap = cv2.VideoCapture('C:\\Users\\Soma2\\myworkspace\\RapidCheck\\python\\opencv\\js\\darkeras\\test\\my_testset\\'+video_name)
cv2.namedWindow('Detection Window',cv2.WINDOW_NORMAL)
cv2.resizeWindow('Detection Window', 600,600)
# cv2.namedWindow('Compare Window', cv2.WINDOW_NORMAL)
# cv2.resizeWindow('Compare Window', 600, 600)
try:
	while True:
		ret, frame = cap.read()
		frameNum += 1
		# if frameNum <= 64000:
		# 	continue
		if frameNum % frameSteps != 0:
			continue
	
		print("FrameNum : {}".format(frameNum))
		# frame2 = frame.copy()
		img = preprocess(frame)
		batch = np.expand_dims(img, axis=0)
		net_out = model.predict(batch)
		out_img, objects, is_object = post_progress(net_out[0], im=frame, is_save=False, threshold=test_threshold)

		# net_out2 = model2.predict(batch)
		# out_img2, objects2, is_object2 = post_progress(net_out2[0], im=frame2, is_save=False, threshold=0.2)
		# if is_object:
		# 	for each_object in objects:
		# 		items.append(DB_Item(videoId, frameNum, each_object[0], each_object[1], each_object[2], each_object[3], each_object[4], each_object[5]))
		
		# if len(items) >= 100:
		# 	db.insert(items)
		# 	print("DB Insert 100 items Done..********************************")
		# 	del items
		# 	items = []
		# cv2.imshow('Compare Window', out_img2)
		cv2.imshow('Detection Window', out_img)
		wk = cv2.waitKey(1)
		if wk & 0xFF == ord('q'):
			break
		elif wk & 0xFF == ord(' '):
			cv2.waitKey(0)
except Exception:
	print("Exception Occured")
	exit(-1)
finally:
	db.close()
	print("DB Closed in Finally..")
	cap.release()
	cv2.destroyAllWindows()

exit(0)

# db.insert(items)
# print("DB Insert 100 items Done..")
# del items
