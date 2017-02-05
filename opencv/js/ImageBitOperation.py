import numpy as np
import cv2


def bit_operation(xpos, ypos):
	img_back = cv2.imread('cute.png')
	img_logo = cv2.imread('opencv-logo.png')

	# cv2.imshow('window', img_back)
	# cv2.imshow('window2', img_logo)

	row, col, ch = img_logo.shape
	roi = img_back[xpos:xpos+row, ypos:col+ypos]

	logo2gray = cv2.cvtColor(img_logo, cv2.COLOR_BGR2GRAY)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

if __name__ == '__main__':
		bit_operation(10, 10)