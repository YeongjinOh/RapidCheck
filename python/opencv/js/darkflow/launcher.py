from net.build import TFNet
import numpy as np
import cv2
from os import sep

options = {"model": "cfg{}yolo-tiny.cfg".format(sep),
		"load":"",
		"threshold": 0.1,
		"gpu":1.0}

tfnet = TFNet(options)
im  = cv2.imread('test/dog.jpg')
print(type(im), im.shape)
print(tfnet.return_predict(im))
