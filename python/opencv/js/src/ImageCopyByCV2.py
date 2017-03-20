import cv2
import numpy
import matplotlib.pyplot as plt

def handle_image():
	img_path = './cute_copy.png'
	img = cv2.imread(img_path, cv2.IMREAD_COLOR)

	cv2.namedWindow('CUTE', cv2.WINDOW_NORMAL)
	cv2.imshow('CUTE', img)
	kb = cv2.waitKey(0) & 0xFF
	if kb == 27:
		cv2.destroyAllWindows()
	elif kb == ord('s'):
		cv2.imwrite('cute_copy2.png', img)
		cv2.destroyAllWindows()
	

if __name__ == '__main__':
	handle_image()