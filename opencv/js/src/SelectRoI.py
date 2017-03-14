import cv2
import numpy as np

if __name__ == '__main__':
	# Read image
	im = cv2.imread("../images/human_image1.jpg")
	 
	# Select ROI
	r = cv2.selectROI(im)
	# print(r, type(r))
	# Crop image
	# imCrop = im[int(r[1]):int(r[1]+r[3]), int(r[0]):int(r[0]+r[2])]

	# Display cropped image
	cv2.imshow("Image", im)
	cv2.waitKey(0)
	cv2.destroyAllWindows()