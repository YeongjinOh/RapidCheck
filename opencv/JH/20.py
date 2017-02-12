import cv2
import numpy as np

def contour():
	img = cv2.imread('test.png')
	imgray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

	ret, thresh = cv2.threshold(imgray, 127,255,0)
	something, contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

	cnt = contours[2]

	moments = cv2.moments(cnt)
	cx = int(moments['m10']/moments['m00'])
	cy = int(moments['m01']/moments['m00'])

	x,y,w,h = cv2.boundingRect(cnt)
	rect_area = w*h
	area = cv2.contourArea(cnt)
	hull = cv2.convexHull(cnt)
	hull_area = cv2.contourArea(hull)
	ellipse = cv2.fitEllipse(cnt)

	aspect_ratio = float(w)/h
	extent = float(area)/rect_area
	solidity = float(area)/hull_area

	print 'Aspect Ratio of ...: %f' %aspect_ratio
	print 'Extent of ...: %f' %extent
	print 'Solidity of ...: %f' %solidity
	print 'Orientation of ...: %f' %ellipse[2]

	equivalent_diameter = np.sqrt(4*area/np.pi)
	radius = int(equivalent_diameter/2)

	cv2.circle(img, (cx,cy),3,(0,0,255),-1)
	cv2.circle(img, (cx,cy),radius,(0,255,0),2)
	cv2.rectangle(img, (x,y),(x+w,y+h),(0,255,0),2)
	cv2.ellipse(img,ellipse, (50,50,50),2)

	cv2.imshow('contour',img)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

if __name__ == '__main__':
	contour()