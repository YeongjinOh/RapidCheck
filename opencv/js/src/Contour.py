import numpy as np
import cv2
from my_cv_lib import show_pictures


def contour():
	img = cv2.imread('../images/cctv_example1.jpg')
	img1 = img.copy()
	img2 = img.copy()
	img3 = img.copy()
	img4 = img.copy()
	gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

	ret, thresh = cv2.threshold(gray_img, 127, 255, 0)
	th, contours_tree, hierarchy1 = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
	th, contours_external, hierarchy2 = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
	th, contours_list, hierarchy3 = cv2.findContours(thresh, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
	th, contours_ccomp, hierarchy4 = cv2.findContours(thresh, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_SIMPLE)

	print(type(contours_tree), type(hierarchy1))
	print('Tree : ', len(contours_tree), len(hierarchy1))
	print('External : ', len(contours_external), len(hierarchy2))
	print('List : ', len(contours_list), len(hierarchy3))
	print('ccomp : ', len(contours_ccomp), len(hierarchy4))

	cv2.drawContours(img1, contours_tree, -1, (0, 255, 0), 1)
	cv2.drawContours(img2, contours_external, -1, (0, 255, 0), 1)
	cv2.drawContours(img3, contours_list, -1, (0, 255, 0), 1)
	cv2.drawContours(img4, contours_ccomp, -1, (0, 255, 0), 1)
	show_pictures(plist=[gray_img, thresh, img1, img2, img3, img4], 
		titles=['Gray Img', 'Thresh', 'Contour Tree', 'Contour External', 'Contour List', 'Contour ccomp'])
if __name__ == '__main__':
	contour()