import cv2
import numpy as np
from matplotlib import pyplot as plt

def histogram():
	img = cv2.imread('hist1.jpg',0)
	hist, bins = np.histogram(img.ravel(), 256,[0,256])
	cdf = hist.cumsum()

	cdf_m = np.ma.masked_equal(cdf,0)
	cdf_m = (cdf_m - cdf_m.min())*255/(cdf_m.max()-cdf_m.min())

	cdf=np.ma.filled(cdf_m,0).astype('uint8')

	img2 = cdf[img]

	cv2.imshow('histo equalization',img2)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

def histogram2():
	img = cv2.imread('hist1.jpg',0)

	equ = cv2.equalizeHist(img)
	res = np.hstack((img,equ))
	cv2.imshow('equalizer',res)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

def histogram3():
	img = cv2.imread('hist1.jpg',0)
	equ = cv2.equalizeHist(img)
	res = np.hstack((img,equ))
	cv2.imshow('equalizer',res)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

if __name__ == '__main__':
	# histogram()
	# histogram2()
	histogram3()