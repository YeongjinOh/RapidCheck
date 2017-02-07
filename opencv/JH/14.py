import cv2
import numpy as np
from matplotlib import pyplot as plt

def grad():
	img = cv2.imread('test.png',0)

	laplacian = cv2.Laplacian(img,cv2.CV_64F)
	sobelx = cv2.Sobel(img, cv2.CV_64F, 1, 0, ksize=3)
	sobely = cv2.Sobel(img, cv2.CV_64F, 0, 1, ksize=3)

	plt.subplot(2,2,1), plt.imshow(img,cmap='gray')
	plt.title('original'), plt.xticks([]), plt.yticks([])
	plt.subplot(2,2,2), plt.imshow(laplacian,cmap='gray')
	plt.title('Laplacian'), plt.xticks([]), plt.yticks([])
	plt.subplot(2,2,3), plt.imshow(sobelx,cmap='gray')
	plt.title('Sobel X'), plt.xticks([]), plt.yticks([])
	plt.subplot(2,2,4), plt.imshow(sobely,cmap='gray')
	plt.title('Sobel Y'), plt.xticks([]), plt.yticks([])
	plt.show()

def grad2():
	img = cv2.imread('box.png',0)

	sobelx8u = cv2.Sobel(img, cv2.CV_8U, 1,0, ksize = 5)

	tmp = cv2.Sobel(img, cv2.CV_64F, 1, 0, ksize=5)
	sobel64f = np.absolute(tmp)
	sobelx8u2 = np.uint8(sobel64f)

	plt.subplot(1,3,1), plt.imshow(img, cmap='gray')
	plt.title('original'), plt.xticks([]), plt.yticks([])
	plt.subplot(1,3,2), plt.imshow(sobelx8u, cmap='gray')
	plt.title('sobel 8U'), plt.xticks([]), plt.yticks([])
	plt.subplot(1,3,3), plt.imshow(sobelx8u2, cmap='gray')
	plt.title('sobel64f'), plt.xticks([]), plt.yticks([])

	plt.show()
if __name__ == '__main__':
	grad2()