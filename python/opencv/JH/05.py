import numpy as np
import cv2
from matplotlib import pyplot as plt
from random import shuffle

b = [i for i in range(256)]
g = [i for i in range(256)]
r = [i for i in range(256)]

def onMouse(event, x , y, flags, param):
	if event ==  cv2.EVENT_LBUTTONDBLCLK:
		shuffle(b),shuffle(g), shuffle(r)
		cv2.circle(param,(x,y),50,(b[0],g[0],r[0]),-1)

def mouseBrush():
	img = np.zeros((512,512,3),np.uint8)
	cv2.namedWindow('paint')
	cv2.setMouseCallback('paint',onMouse,param=img)

	while (1):
		cv2.imshow('paint',img)
		k = cv2.waitKey(1) & 0xFF

		if k == 27:
			break
	cv2.destroyAllWindows()

if __name__ == '__main__':
	mouseBrush()
