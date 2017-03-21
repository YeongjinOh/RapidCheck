import numpy as np
import cv2
from my_cv_lib import show_pictures

def threshing():
	img = cv2.imread('cctv_example1.jpg', 0)

	ret, th1 = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)
	th2 = cv2.adaptiveThreshold(img, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 11, 2)
	th3 = cv2.adaptiveThreshold(img, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)
	ret2, th4 = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
	blur = cv2.GaussianBlur(img, (5, 5), 0)
	ret3, th5 = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
	# titles = ['original', 'Global Threshing (v=127)', 'Adaptive Mean', 'Adaptive Gaussian']
	# plist = [img, th1, th2, th3]
	plist = [img, th4, blur, th5]
	titles = ['original', 'otsu', 'G blur', 'G blur otsu']

	show_pictures(plist, titles=titles)

if __name__ == '__main__':
	threshing()