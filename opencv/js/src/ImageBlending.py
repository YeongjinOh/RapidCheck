import numpy as np
import cv2
from skimage.transform import resize

def onMouse(x):
	pass

def blending():
	img1 = cv2.imread('cute.png')
	img2 = cv2.imread('cute2.png')
	r, c, _ = img1.shape
	print(img1.shape, r, c)
	img2 = resize(img2, (r,c,3))
	print(img1.shape, img2.shape)
	img2.dtype = np.uint8
	print(img1.dtype, img2.dtype)
	print(img1.ndim, img2.ndim)
	# cv2.imshow('window1', img1)
	# cv2.imshow('window2', img2)
	print(img1[0:2, 0:2])
	print(img2[0:2, 0:2])
	
	print(len(img1), len(img2))
	return

	cv2.namedWindow('window')
	cv2.createTrackbar('Mixing', 'window', 0, 100, onMouse)
	mix = cv2.getTrackbarPos('Mixing', 'window')

	while True:
		img = cv2.addWeighted(img1, float(100-mix)/100, img2, float(mix)/100, 0)
		cv2.imshow('window', img)

		if cv2.waitKey(1) & 0xFF == 27:
			break

		mix = cv2.getTrackbarPos('Mixing', 'window')
		

	# cv2.waitKey(0)
	cv2.destroyAllWindows()

if __name__ == '__main__':
	blending()