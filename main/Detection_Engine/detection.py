
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

K.set_image_dim_ordering('th')

is_freeze = True
# weigths_path = 'models/train/yolo-2class-complete.h5'
# weigths_path = os.path.join(cfg.model_folder, cfg.model_name) + '-steps8000.h5'
weigths_path = 'models/train/yolo-2class-mydata-tracking-cell14-steps44000.h5'
# weigths_path = 'models/train/'+cfg.model_name+'-complete.h5'
# weigths_path = 'models/train/yolo-2class-mydata-3video-steps5000.h5'
test_threshold = 0.4

# weigths_path = 'models/train/yolo-2class-voc2007-train-cell28-steps40000.h5'
# weigths_path = 'models/train/yolo-2class-mydata-3video-steps5000.h5'
model = yolo_tiny_THdim_model(is_freeze)
# model = yolo_shortdense_THdim_model(is_freeze)
model.load_weights(weigths_path)
#model.summary()


frameNum = 0
items = []
cap = cv2.VideoCapture(video_path)
try:
	while True:
		ret, frame = cap.read()
		frameNum += 1

		## TODO
		if frameNum > 100:
		 	break
		 	
		if frameNum % frameSteps != 0:
			continue
	
		print("FrameNum : {}".format(frameNum))
		img = preprocess(frame)
		batch = np.expand_dims(img, axis=0)
		net_out = model.predict(batch)
		out_img, objects, is_object = post_progress(net_out[0], im=frame, is_save=False, threshold=test_threshold)

		if is_object:
			for each_object in objects:
				items.append(DB_Item(videoId, frameNum, each_object[0], each_object[1], each_object[2], each_object[3], each_object[4], each_object[5]))
		
		if len(items) >= 100:
			db.insert(items, table_name='detection')
			print("DB Insert 100 items Done..********************************")
			del items
			items = []

except Exception:
	print("Exception Occured")
	exit(-1)
finally:
	db.insert(items, table_name='detection')
	db.close()
	print("DB Closed in Finally..")
	cap.release()
	
exit(0)
