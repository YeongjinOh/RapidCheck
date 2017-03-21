import numpy as np
import cv2

img = cv2.imread('cute.png')
px = img[100, 100]
print(px)
b_px = img[100, 100, 0]
print("blue px : ", b_px)

# Region of Image (ROI) 

if __name__ == '__main__':
	img = cv2.imread('cute.png')
	print(img.shape)
	cv2.imshow('window', img)

	part = img[50:100, 50:100]
	cv2.imshow('cutting', part)

	img[150:200, 150:200] = part

	print(img.shape)
	print(part.shape)

	cv2.imshow('modifying', img)

	cv2.waitKey(0)
	cv2.destroyAllWindows()
