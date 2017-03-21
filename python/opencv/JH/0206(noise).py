import cv2
import time
from matplotlib import pyplot as plt

if __name__ == '__main__':
	cap = cv2.VideoCapture('1.avi')
	print cap.isOpened()
	mog = cv2.createBackgroundSubtractorMOG2(500,0,True)
	while(cap.isOpened()):
		time.sleep(0.2)
		ret, frame = cap.read()
		fgmask = mog.apply(frame)
		
		blur = cv2.blur(fgmask,(5,5))
		median = cv2.medianBlur(fgmask, 3)
		bilater = cv2.bilateralFilter(
			
			fgmask,3,15,15)
		gau = cv2.GaussianBlur(fgmask, (5,5),0)
		
		cv2.imshow('test',fgmask)
		cv2.imshow('blur', blur)
		cv2.imshow('median', median)
		cv2.imshow('bilater', bilater)
		cv2.imshow('gaussian', gau)

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break
	cap.release()
	cv2.destroyAllWindows()