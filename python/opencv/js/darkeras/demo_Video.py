
import os
import glob
import h5py
import numpy as np
import cv2
import matplotlib.pyplot as plt
from keras import backend as K

import tensorflow as tf
import yolo.config as cfg

from yolo.net.yolo_tiny_thdim_net import yolo_tiny_THNet
from utils.BoxUtils import post_progress
from yolo.process import preprocess

K.set_image_dim_ordering('th')

is_freeze = True
# weigths_path = 'models/train/yolo-2class-epoch50.h5'
weigths_path = 'models/train/yolo-2class-complete.h5'
test_threshold = 0.2

model = yolo_tiny_THNet(is_freeze)
model.load_weights(weigths_path)
model.summary()

# video_name = 'persons1.mp4'
video_name = 'tracking.mp4'
cap = cv2.VideoCapture('C:\\Users\\SoMa\\myworkspace\\RapidCheck\\python\\opencv\\js\\darkeras\\test\\my_testset\\'+video_name)
while True:
	ret, frame = cap.read()
	# cv2.imshow('Original Window', frame)
	img = preprocess(frame)
	batch = np.expand_dims(img, axis=0)
	net_out = model.predict(batch)
	out_img = post_progress(net_out[0], im=frame, is_save=False, threshold=test_threshold)
	cv2.imshow('Detection Window', out_img)
	if cv2.waitKey(1) & 0xFF == ord('q'):
		break

cap.release()
cv2.destroyAllWindows()