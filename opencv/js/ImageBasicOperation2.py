import numpy as np
import cv2

if __name__ == '__main__':
	img = cv2.imread('cute.png')

	b, g, r = cv2.split(img)

	print(b.shape, g.shape, r.shape)

	print(img[100, 100])
	print(b[100, 100], g[100,100], r[100,100])

	cv2.imshow('b-window', b)
	cv2.imshow('g-window', g)
	cv2.imshow('r-window', r)
	cv2.imshow('total-window', cv2.merge((b,g,r)))

	"""
	cv2.split 은 성능이 좋지 않다. 만약 필요해서 사용하게 된다면, numpy 의 slice 기능을 이용하는 것을 추천한다.
	b = img[:, :, 0]
	b = img[:, :, 1]
	b = img[:, :, 2]
	"""
	
	cv2.waitKey(0)
	cv2.destroyAllWindows()
