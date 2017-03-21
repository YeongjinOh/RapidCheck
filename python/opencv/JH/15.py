import cv2
import numpy as np

def canny():
	img = cv2.imread('test.png',0)
	edges1 =cv2.Canny(img,70,300)
	edges2 =cv2.Canny(img,100,200)
	edges3 =cv2.Canny(img,170,200)

	cv2.imshow('original',img)
	cv2.imshow('canny 1',edges1)
	cv2.imshow('canny 2',edges2)
	cv2.imshow('canny 3',edges3)
        
	cv2.waitKey(0)
	cv2.destroyAllWindows()

if __name__ == '__main__':
	canny()
