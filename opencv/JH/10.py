import cv2
from matplotlib import pyplot as plt
# if __name__ == '__main__':
# 	img = cv2.imread('500by500.png',0)

# 	ret, thresh1 = cv2.threshold(img, 127,255, cv2.THRESH_BINARY)
# 	ret, thresh2 = cv2.threshold(img, 127,255, cv2.THRESH_BINARY_INV)
# 	ret, thresh3 = cv2.threshold(img, 127,255, cv2.THRESH_TRUNC)
# 	ret, thresh4 = cv2.threshold(img, 127,255, cv2.THRESH_TOZERO)
# 	ret, thresh5 = cv2.threshold(img, 127,255, cv2.THRESH_TOZERO_INV)

# 	cv2.imshow('original',img)
# 	cv2.imshow('BINARY',thresh1)
# 	cv2.imshow('BINARY_INV',thresh2)
# 	cv2.imshow('TRUNC',thresh3)
# 	cv2.imshow('TOZERO',thresh4)
# 	cv2.imshow('TOZERO_INV',thresh5)

# 	cv2.waitKey(0)
# 	cv2.destroyAllWindows()

def thresholding():
	img = cv2.imread('test.png',0)

	ret, th1 = cv2.threshold(img, 127,255, cv2.THRESH_BINARY)
	th2 = cv2.adaptiveThreshold(img,255, cv2.ADAPTIVE_THRESH_MEAN_C,cv2.THRESH_BINARY,11,2)
	th3 = cv2.adaptiveThreshold(img,255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,11,2)

	titles = ['original', 'Global Thresholding (v=127)','Adaptive MEAN', 'Adaptive GAUSSIAN']
	images = [img,th1,th2,th3]

	for i in xrange(4):
		cv2.imshow(titles[i],images[i])

	cv2.waitKey(0)
	cv2.destroyAllWindows()


def thresholding2():
	img = cv2.imread('test.png',0)

	ret, th1 = cv2.threshold(img, 127,255, cv2.THRESH_BINARY) # ret = 127

	ret2, th2 = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
	# print ret2 # 162

	blur = cv2.GaussianBlur(img, (5,5),0)
	ret3, th3 = cv2.threshold(blur,0,255,cv2.THRESH_BINARY + cv2.THRESH_OTSU)

	titles = ['original noisy', 'Histogram','G-Thresholding',\
	'original noisy', 'Histogram','Otsu Thresholding','Gaussian-filtered','Histogram','Otsu Thresholding']
	images = [img, 0,th1,img,0,th2,blur, 0, th3]

	for i in xrange(3):
		plt.subplot(3,3,i*3+1), plt.imshow(images[i*3],'gray')
		plt.title(titles[i*3]),plt.xticks([]), plt.yticks([])

		plt.subplot(3,3,i*3+2), plt.hist(images[i*3].ravel(),256)
		plt.title(titles[i*3+1]),plt.xticks([]), plt.yticks([])

		plt.subplot(3,3,i*3+3), plt.imshow(images[i*3+2],'gray')
		plt.title(titles[i*3+2]),plt.xticks([]), plt.yticks([])

	plt.show()

if __name__ == '__main__':
	# thresholding()
	thresholding2()
