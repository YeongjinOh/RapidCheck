import cv2
import numpy as np

def onChange(x):
	pass

def trackbar():
	img = np.zeros((300,512,3),np.uint8)
	cv2.namedWindow('color_palette')

	cv2.createTrackbar('B','color_palette',0,255,onChange)
	cv2.createTrackbar('G','color_palette',0,255,onChange)
	cv2.createTrackbar('R','color_palette',0,255,onChange)

	switch = '0 : OFF \n1 : ON'
	cv2.createTrackbar(switch,'color_palette',0,1,onChange)

	while 1:
		cv2.imshow('color_palette',img)
		k = cv2.waitKey(1) & 0xFF
		if k == 27:
			break
		b = cv2.getTrackbarPos('B','color_palette')
		g = cv2.getTrackbarPos('G','color_palette')
		r = cv2.getTrackbarPos('R','color_palette')
		s = cv2.getTrackbarPos(switch,'color_palette')

		if s == 0:
			img[:] = 0
		else:
			img[:] = [b,g,r]

	cv2.destroyAllWindows()
if __name__ == '__main__':
	trackbar()