import numpy as np
import cv2

view_mode = False

def onMouse(event, x, y, flags, param):
	print("onMouse..")
	if event == cv2.EVENT_LBUTTONDOWN:
		global view_mode
		if view_mode is True:
			view_mode = False
		else:
			view_mode = True


def backSub():
	cap = cv2.VideoCapture(0)
	mog = cv2.createBackgroundSubtractorMOG2()
	img = np.zeros((512, 512, 3), np.uint8)

	cv2.namedWindow('window')
	cv2.setMouseCallback('window', onMouse, param=img)
	cv2.imshow('window', img)
	
	while True:
		ret, frame = cap.read()
		fgmask = mog.apply(frame)
		if ret is True:
			if view_mode is True:
				cv2.imshow('window', fgmask)
			else:
				cv2.imshow('window', frame)

		if cv2.waitKey(30) & 0xFF == 27:
			break


	cap.release()
	cv2.destroyAllWindows()

if __name__ == '__main__':
	backSub()