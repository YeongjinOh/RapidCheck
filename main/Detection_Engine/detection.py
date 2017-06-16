
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

from utils.help import DB_Helper, DB_Item
import os
import argparse

# root_path = '../../../Detection_Engine/'
# os.chdir(root_path)

parser = argparse.ArgumentParser()
parser.add_argument('--videoId', type=int, help='videoId for detecting works', required=True)
parser.add_argument('--videoPath', help='VideoPath for detecting works', required=True)
parser.add_argument('--frameSteps', type=int, help='frameSteps for detecting works', required=True)
parser.add_argument('--maxFrame', type=int, help='MaxFrame for detecting works', required=True)
args = parser.parse_args()
# db.delete()
if args.videoPath and args.maxFrame and args.videoId and args.frameSteps:
	videoId = args.videoId
	video_path = args.videoPath
	frameSteps = args.frameSteps
	maxFrame = args.maxFrame
	print("videoPath : ", video_path)
	
	# db = DB_Helper()
	# db.open(table_name='file')
	# row = db.select(table_name='file', condition={'videoId':videoId})
	# video_path = row['videoPath']
	# frameSteps = row['frameSteps']
db = DB_Helper()
db.open(table_name='detection')

K.set_image_dim_ordering('th')
is_freeze = True
# weigths_path = 'models/train/yolo-2class-complete.h5'
# weigths_path = os.path.join(cfg.model_folder, cfg.model_name) + '-steps8000.h5'
# weigths_path = 'models/train/yolo-2class-mydata-tracking-cell14-steps44000.h5'
# weigths_path = 'dropbox/models/train/yolo-2class-cell14-mydata100000/only-video_439532-steps8000.h5'
# weigths_path = 'dropbox/models/train/yolo-2class-cell14-vocbase/from-cell14base-mydata-steps100000.h5'
# weigths_path = 'dropbox/models/train/yolo-2class-cell14-mydata100000/from-cell14-mydata-tracking-mydata-20000-steps8000.h5'
weigths_path = 'dropbox/models/train/yolo-2class-cell14-mydata100000/from-mydatabase-mot-person-steps16000.h5'
# weigths_path = 'models/train/'+cfg.model_name+'-complete.h5'
# weigths_path = 'models/train/yolo-2class-mydata-3video-steps5000.h5'
test_threshold = 0.4
# video_path = os.path.join('dropbox', 'dataset', 'datacenter', 'video_439532', 'video_439532.mp4')
# video_path = os.path.join('dropbox', 'dataset', 'datacenter', 'video_166497', 'video_166497.mp4')
# video_path = os.path.join('dropbox', 'dataset', 'datacenter', 'video_716195', 'video_716195.mp4')

# weigths_path = 'models/train/yolo-2class-voc2007-train-cell28-steps40000.h5'
# weigths_path = 'models/train/yolo-2class-mydata-3video-steps5000.h5'
model = RCNet_THdim_model(is_freeze)
# model = yolo_shortdense_THdim_model(is_freeze)
model.load_weights(weigths_path)
#model.summary()

frameNum = -1
items = []
cap = cv2.VideoCapture(video_path)
try:
	while True:
		ret, frame = cap.read()
		frameNum += 1
		
		if frameNum > maxFrame or not ret:
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
		
		if len(items) >= 10:
			db.insert(items, table_name='detection')
			print("DB Insert 10 items Done..********************************")
			del items
			items = []

		print("RapidCheck_Detection {}".format(int(frameNum/maxFrame*1000)))
		

except Exception:
	print("Exception Occured")
	exit(-1)
finally:
	db.insert(items, table_name='detection')
	db.close()
	print("DB Closed in Finally..")
	print("RapidCheck_Detection {}".format(1000))
	cap.release()
	
exit(0)
