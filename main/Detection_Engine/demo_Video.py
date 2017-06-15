
import os
import glob
import h5py
import numpy as np
import cv2
import matplotlib.pyplot as plt
from keras import backend as K

import tensorflow as tf
import yolo.config as cfg

from yolo.net.RCNet_thdim_net import RCNet_THdim_model, RCNet_shortdense_THdim_model, RCNet_THdim_dropout_model
from utils.BoxUtils import post_progress
from yolo.process import preprocess

# from utils.help import DB_Helper, DB_Item

# import argparse

# parser = argparse.ArgumentParser()
# parser.add_argument('--videoId', type=int, help='VideoId for detecting works', required=True)
# args = parser.parse_args()
# # db.delete()
# if args.videoId:
# 	videoId = args.videoId
# 	print("video id : ", args.videoId)
# 	# TODO : query video path from mysql
# 	db = DB_Helper()
# 	db.open(table_name='file')
# 	row = db.select(table_name='file', condition={'videoId':videoId})
# 	video_path = row['videoPath']
# 	frameSteps = row['frameSteps']
#exit()
K.set_learning_phase(1) #set learning phase

if cfg.image_dim_order == 'th':
	K.set_image_dim_ordering('th')

is_freeze = True
# weigths_path = 'models/train/yolo-2class-complete.h5'
# weigths_path = os.path.join(cfg.model_folder, cfg.model_name) + '-steps8000.h5'
# weigths_path = 'models/train/yolo-2class-mydata-tracking-cell14-steps44000.h5'
# weigths_path = 'dropbox/models/train/yolo-2class-cell14-voc-dropout/dropout-mydata-train-steps48000.h5'
# weigths_path = 'dropbox/models/train/yolo-2class-cell14-mydata100000/only-video_439532-steps8000.h5'
weigths_path = 'dropbox/models/train/yolo-2class-cell14-vocbase/from-cell14base-mydata-steps100000.h5'
# weigths_path = 'dropbox/models/train/yolo-2class-cell14-mydata100000/from-cell14-mydata-tracking-mydata-20000-steps8000.h5'
# weigths_path = 'models/train/yolo-2class-mydata-3video-steps5000.h5'
test_threshold = 0.4

# weigths_path = 'models/train/yolo-2class-voc2007-train-cell28-steps40000.h5'
# weigths_path = 'models/train/yolo-2class-mydata-3video-steps5000.h5'
# model = RCNet_THdim_dropout_model(is_freeze)
model = RCNet_THdim_model(is_freeze)
# model = RCNet_shortdense_THdim_model(is_freeze)
model.load_weights(weigths_path)
#model.summary()

# video_path = os.path.join('dropbox', 'dataset', 'datacenter', 'video_439532', 'video_439532.mp4')
video_path = os.path.join('dropbox', 'dataset', 'datacenter', 'video_166497', 'video_166497.mp4')
# video_path = os.path.join('dropbox', 'dataset', 'datacenter', 'video_716195', 'video_716195.mp4')
frameSteps = 1
frameNum = -1
items = []
cap = cv2.VideoCapture(video_path)
print (video_path)
cv2.namedWindow('Detection Window',cv2.WINDOW_NORMAL)
cv2.resizeWindow('Detection Window', 600,600)

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
		
		# cv2.imshow('Compare Window', out_img2)
		cv2.imshow('Detection Window', out_img)
		wk = cv2.waitKey(1)
		if wk & 0xFF == ord('q'):
			break
		elif wk & 0xFF == ord(' '):
			cv2.waitKey(0)
		
except Exception:
	print("Exception Occured")

finally:

	cap.release()
	cv2.destroyAllWindows()

