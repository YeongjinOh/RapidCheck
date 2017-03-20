import numpy as np
import cv2
from my_cv_lib import show_pictures

def convex():
	img = cv2.imread('../images/convexhull.png')
	gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

	ret, threshold = cv2.threshold(gray_img, 127, 255, 0)
	th, contours, hierarchy = cv2.findContours(threshold, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

	cnt = contours[1]
	cv2.drawContours(img, [cnt], 0, (0, 255, 0), 3)

	check = cv2.isContourConvex(cnt)

	if check == False:
		img1 = img.copy()
		hull = cv2.convexHull(cnt)
		cv2.drawContours(img1, [hull], 0, (0, 255, 0), 3)
		cv2.imshow('convexhull', img1)

	cv2.imshow('contour', img)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

if __name__ == '__main__':
	convex()