import cv2
import numpy as np
from matplotlib import pyplot as plt
def histogram():
	img1 = cv2.imread('landscape.jpg',0)
	img2 = cv2.imread('landscape.jpg')

	hist1 = cv2.calcHist([img1],[0],None,[256],[0,256])

	hist2, bins = np.histogram(img1.ravel(),256,[0,256])

	hist3 = np.bincount(img1.ravel(),minlength=256)

	plt.hist(img1.ravel(),256,[0,256])

	color = ('b','g','r')

	for i, col in enumerate(color):
		hist = cv2.calcHist([img2],[i],None,[256],[0,256])
		plt.plot(hist, color = col)
		plt.xlim([0,256])

	plt.show()

if __name__ == '__main__':
	histogram()