import numpy as np
import cv2

def handle_video(dev=0):
	cap = cv2.VideoCapture(dev)

	fps = 20.0
	width = int(cap.get(3))
	height = int(cap.get(4))
	fcc = cv2.VideoWriter_fourcc(*('DIVX'))

	out = cv2.VideoWriter('Cam_copy.avi', fcc, fps, (width, height))

	while cap.isOpened():
		ret, frame = cap.read()
		print(ret)
		if ret == True:
			out.write(frame)
			cv2.imshow('FRAME', frame)
			# gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
			if cv2.waitKey(1) & 0xFF == ord('q'):
				break
		else:
			break

	cap.release()
	out.release()
	cv2.destroyAllWindows()


if __name__ == '__main__':
	handle_video()