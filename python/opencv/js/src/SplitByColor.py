import numpy as np
import cv2
from my_cv_lib import show_pictures

def split_frame_color():
	try:
		cap = cv2.VideoCapture(0)
	except:
		print("cannot catch video")
		return
	while True:
		ret, frame = cap.read()
		hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

		blue_lower = np.array([110, 100, 100])
		blue_upper = np.array([130, 255, 255])

		green_lower = np.array([50, 100, 100])
		green_upper = np.array([70, 255, 255])

		red_lower = np.array([-10, 100, 100])
		red_upper = np.array([10, 255, 255])

		mask_blue = cv2.inRange(hsv, blue_lower, blue_upper)
		mask_green = cv2.inRange(hsv, green_lower, green_upper)
		mask_red = cv2.inRange(hsv, red_lower, red_upper)

		res_blue = cv2.bitwise_and(frame, frame, mask=mask_blue)
		res_green = cv2.bitwise_and(frame, frame, mask=mask_green)
		res_red = cv2.bitwise_and(frame, frame, mask=mask_red)

		# plist = [frame, res_blue]
		# show_pictures(plist, block=False)
		cv2.imshow('original', frame)
		cv2.imshow('blue', res_blue)
		cv2.imshow('red', res_red)
		cv2.imshow('green', res_green)

		if (cv2.waitKey(5) & 0xFF) == 27:
			break

	cap.release()
	cv2.destroyAllWindows()
if __name__ == '__main__':
	split_frame_color()