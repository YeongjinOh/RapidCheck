import cv2
import numpy as np

# if __name__ == '__main__':
# 	blue = np.uint8([[[255,0,0]]])
# 	green = np.uint8([[[0,255,0]]])
# 	red = np.uint8([[[0,0,255]]])

# 	hsv_blue = cv2.cvtColor(blue,cv2.COLOR_BGR2HSV)
# 	hsv_green = cv2.cvtColor(green,cv2.COLOR_BGR2HSV)
# 	hsv_red = cv2.cvtColor(red,cv2.COLOR_BGR2HSV)

# 	print hsv_blue, hsv_green, hsv_red
# 	#[[[120 255 255]]] [[[ 60 255 255]]] [[[  0 255 255]]]

def tracking():
	try:
		print 'start camera'
		cap = cv2.VideoCapture(0)
	except:
		print 'cannot load camera'
		return

	while 1:
		ret, frame = cap.read()

		hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

		#define range of blue color in HSV
		lower_blue = np.array([110,100,100])
		upper_blue = np.array([130,255,255])

		lower_green = np.array([50,100,100])
		upper_green = np.array([70,255,255])

		lower_red = np.array([-10,100,100])
		upper_red = np.array([10,255,255])

		#threshold the HSV image to get only blue colors
		mask_blue = cv2.inRange(hsv, lower_blue,upper_blue)
		mask_green = cv2.inRange(hsv, lower_green,upper_green)
		mask_red = cv2.inRange(hsv, lower_red,upper_red)

		#bitwise_and mask and original image
		res1 = cv2.bitwise_and(frame,frame,mask=mask_blue)
		res2 = cv2.bitwise_and(frame,frame,mask=mask_green)
		res3 = cv2.bitwise_and(frame,frame,mask=mask_red)

		cv2.imshow('original',frame)
		cv2.imshow('blue',res1)
		cv2.imshow('green',res2)
		cv2.imshow('red',res3)

		k = cv2.waitKey(5) & 0xFF
		if k == 27:
			break
	cv2.destroyAllWindows()

if __name__ == '__main__':
	tracking()