import numpy as np
import cv2
from random import shuffle

B = [i for i in range(256)]
G = [i for i in range(256)]
R = [i for i in range(256)]

def onMouse(event, x, y, flags, param):
	print("onMouse..")
	shuffle(B)
	shuffle(G)
	shuffle(R)
	# if event == cv2.EVENT_LBUTTONDBLCLK:
	# 	print("DB click Event")
	# 	cv2.rectangle(param, (x,y), (x+100, y+100), (B[0], G[0], R[0]), -1)
	if event == cv2.EVENT_LBUTTONDOWN:
		print("click Event")
		cv2.circle(param, (x,y), 50, (B[0], G[0], R[0]), -1)
	

def mouseBrush():
	img = np.zeros((512, 512, 3), np.uint8)
	cv2.namedWindow('window')
	cv2.setMouseCallback('window', onMouse, param=img)

	while True:
		cv2.imshow('window', img)
		kb = cv2.waitKey(1) & 0xFF
		print(kb)
		if cv2.waitKey(20) & 0xFF == 27:
			break

	cv2.destroyAllWindows()

if __name__ == '__main__':
	mouseBrush()


