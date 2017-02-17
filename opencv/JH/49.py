import cv2
import numpy as numpy
def backsub():
	cap = cv2.VideoCapture(0)
	mog = cv2.createBackgroundSubtractorMOG2()

	while 1:
		ret, frame = cap.read()
		fgmask = mog.apply(frame)

		cv2.imshow('mask',fgmask)

		k = cv2.waitKey(30) & 0xFF
		if k == 27:
			break
	cap.release()
	cv2.destroyAllWindows()

if __name__ == '__main__':
	backsub()