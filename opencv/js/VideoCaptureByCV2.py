import numpy as np
import cv2

def handle_video(dev=0):
	cap = cv2.VideoCapture(dev)

	while True:
		ret, frame = cap.read()
		print(ret)
		gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		cv2.imshow('FRAME', frame)

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

	cap.release()
	cv2.destroyAllWindows()


if __name__ == '__main__':
	handle_video()