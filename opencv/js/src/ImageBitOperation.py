import numpy as np
import cv2
from my_cv_lib import show_pictures

def bit_operation(xpos, ypos):
	img_back = cv2.imread('cute.png')
	img_logo = cv2.imread('opencv-logo.png')

	# cv2.imshow('window', img_back)
	# cv2.imshow('window2', img_logo)

	row, col, ch = img_logo.shape
	roi = img_back[xpos:xpos+row, ypos:col+ypos]

	logo2gray = cv2.cvtColor(img_logo, cv2.COLOR_BGR2GRAY)
	ret, mask = cv2.threshold(logo2gray, 10, 255, cv2.THRESH_BINARY)
	mask_inv = cv2.bitwise_not(mask)

	# cv2.imshow('window1', mask)
	# cv2.imshow('window2', mask_inv)
	# cv2.moveWindow('window2', 200,0)

	img_bg  = cv2.bitwise_and(roi, roi, mask=mask_inv)
	img_fg = cv2.bitwise_and(img_logo, img_logo, mask=mask)

	# cv2.imshow('window3', img_bg)
	# cv2.moveWindow('window3', 400,0)
	# cv2.imshow('window4', img_fg)
	# cv2.moveWindow('window4', 600,0)

	dst = cv2.add(img_bg, img_fg)
	# cv2.imshow('window5', dst)
	# cv2.moveWindow('window5', 800, 0)

	img_back[xpos:xpos+row, ypos:col+ypos] = dst

	# cv2.imshow('window6', img_back)
	# cv2.moveWindow('window6', 1000, 0)

	# cv2.waitKey(0)
	# cv2.destroyAllWindows()
	plist = [mask, mask_inv, img_bg, img_fg, dst]
	show_pictures(plist, titles=['mask', 'mask_inv', 'img_bg', 'img_fg', 'dst'])

if __name__ == '__main__':
		bit_operation(10, 10)