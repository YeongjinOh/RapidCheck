import numpy as np
import cv2
from my_cv_lib import show_pictures

def erosion(img, show=True):
	kernel = np.ones((3,3), np.uint8)

	erosion_img = cv2.erode(img, kernel, iterations=1)
	if show:
		show_pictures([img, erosion_img], titles=['Original', 'Erosion'])
	return erosion_img

def dilation(img, show=True):
	kernel = np.ones((3,3), np.uint8)

	dilation_img = cv2.dilate(img, kernel, iterations=1)
	if show:
		show_pictures([img, dilation_img], titles=['original', 'Dilation'])
	return dilation_img


def opening(img, show=True, iterations=1):
	"""
	Opening 기법 : Erosion -> Dilation 적용
	물체 외부의 노이즈가 많을때 적합
	"""
	kernel = np.ones((5,5), np.uint8)
	
	opening_list = []
	opening_list.append(img)
	titles = []
	titles.append('Orignal')

	for i in range(1, iterations+1):
		opening_list.append(cv2.morphologyEx(img, cv2.MORPH_OPEN, kernel, iterations=i))
		titles.append('Opening Iter='+str(i))

	if show:
		show_pictures(opening_list, titles=titles)
	
	return opening_list

def closing(img, show=True, iterations=1):
	"""
	Closing 기법 : Dilation -> Erosion 적용
	물체 내부의 노이즈가 많을때 적합
	"""
	kernel = np.ones((5,5), np.uint8)
	
	closing_list = []
	closing_list.append(img)
	titles = []
	titles.append('Orignal')

	for i in range(1, iterations+1):
		closing_list.append(cv2.morphologyEx(img, cv2.MORPH_CLOSE, kernel, iterations=i))
		titles.append('Closing Iter='+str(i))

	if show:
		show_pictures(closing_list, titles=titles)
	
	return closing_list

if __name__ == '__main__':
	# erosion('../images/alpha.png')
	# img = cv2.imread('../images/alpha.png')
	# dilation(img)
	img = cv2.imread('../images/A_noise.png', 0)
	# opening(img, iterations=4)
	img2 = cv2.imread('../images/B_noise.png', 0)
	closing(img2, iterations=4)
