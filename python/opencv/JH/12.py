import cv2
import numpy as np

def bluring():
	img = cv2.imread('test.png')

	kernel = np.ones((5,5),np.float32)/25
	blur = cv2.filter2D(img, -1, kernel)

	cv2.imshow('original',img)
	cv2.imshow('blur',blur)

	cv2.waitKey(0)
	cv2.destroyAllWindows()

def onMouse(x):
	pass

def bluring2():
	img = cv2.imread('test.png')

	cv2.namedWindow('BlurPane')
	cv2.createTrackbar('BLUR_MODE','BlurPane',0,2,onMouse)
	cv2.createTrackbar('BLUR','BlurPane',0,5,onMouse)

	mode = cv2.getTrackbarPos('BLUR_MODE','BlurPane')
	val = cv2.getTrackbarPos('BLUR','BlurPane')

	while 1:
		val = val*2 +1

		try:
			if mode == 0:
				blur = cv2.blur(img,(val,val))
			elif mode == 1:
				blur = cv2.GaussianBlur(img,(val,val),0)
			elif mode == 2:
				blur = cv2.medianBlur(img,val)
			else:
				break

			cv2.imshow('BlurPane',blur)
		except:
			print 'error'
			break

		k = cv2.waitKey(1) & 0xFF
		if k == 27:
			break
			
		mode = cv2.getTrackbarPos('BLUR_MODE','BlurPane')
		val = cv2.getTrackbarPos('BLUR','BlurPane')

	cv2.destroyAllWindows()

if __name__ == '__main__':
	bluring2()