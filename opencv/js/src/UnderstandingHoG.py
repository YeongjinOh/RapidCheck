import numpy as np
import cv2
from my_cv_lib import show_pictures



if __name__ == '__main__':
	img = cv2.imread('../images/bolt.png')
	print(img.shape)
	img = np.float32(img) / 255.0

	# Calculate gradient
	gx = cv2.Sobel(img, cv2.CV_32F, 1, 0, ksize=1)
	gy = cv2.Sobel(img, cv2.CV_32F, 0, 1, ksize=1)

	# Calculate magnitude and direction ( in degrees )
	mag, angle = cv2.cartToPolar(gx, gy, angleInDegrees=True)

	"""
	The gradient image removed a lot of non-essential information ( e.g. constant colored background ), 
	but highlighted outlines. 
	In other words, you can look at the gradient image and still easily say there is a person in the picture.
	"""

	show_pictures([gx, gy, mag, img], 
		titles=['Absolute value of gx', 'Absolute value of gy', 'Magnitude', 'Original'])