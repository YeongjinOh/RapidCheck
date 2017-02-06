import numpy as np
import cv2

def show_pictures(plist, block=True, titles=None):
	for i, item in enumerate(plist):
		if titles is None:
			window_name = 'window'+str(i)
		else:
			window_name = titles[i]
		cv2.imshow(window_name, item)
		cv2.moveWindow(window_name, 200*i, 0)
	if block:
		cv2.waitKey(0)
		cv2.destroyAllWindows()
	return i


if __name__ == '__main__':
	plist = []
	plist.append(cv2.imread('cute.png'))
	plist.append(cv2.imread('cute2.png'))
	show_pictures(plist)