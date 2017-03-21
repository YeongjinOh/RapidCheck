import cv2

def contour():
	img = cv2.imread('aircraft.png')
	imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

	ret, thresh = cv2.threshold(imgray, 127,255,0)
	something, contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

	cv2.imshow('thresh',thresh)
	cv2.drawContours(img,contours, -1, (0,255,0),1)
	cv2.imshow('Contour',img)

	cv2.waitKey(0)
	cv2.destroyAllWindows()

if __name__ == '__main__':
	contour()