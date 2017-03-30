from net.build import TFNet
import numpy as np
import cv2

options = {"model": "cfg\\v1\\yolo-tiny.cfg", 
		"load": "bin\\yolo-tiny.weights", 
		"threshold": 0.1,
		"gpu":1.0}

tfnet = TFNet(options)
