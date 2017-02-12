import cv2
import numpy as np
def convex():
	img = cv2.imread('convexhull.png')

	imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

	ret, thresh = cv2.threshold(imgray, 127, 255, 0)
	something, contours, hierarchy = cv2.findContours(thresh, 1, 2)

	cnt = contours[1]
	cv2.drawContours(img, [cnt],0, (0,255,0),3)

	check = cv2.isContourConvex(cnt)

	if check == False:
		img1 = img.copy()
		hull = cv2.convexHull(cnt)
		cv2.drawContours(img1,[hull],0,(0,255,0),3)
		cv2.imshow('convexhull',img1)
	
	cv2.imshow('contour',img)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

def convex2():
	img = cv2.imread('lightning.png')
	imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

	ret, thresh = cv2.threshold(imgray, 127,255,0)
	something, contours, hierarchy = cv2.findContours(thresh,1,2)

	cnt = contours[4]

	x,y,w,h = cv2.boundingRect(cnt)
	cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255),3)

	rect = cv2.minAreaRect(cnt)
	box = cv2.boxPoints(rect)
	box = np.int0(box)

	print(box)
	cv2.drawContours(img, [box], 0, (0,255,0), 3)

	cv2.imshow('rectangle',img)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

def convex3():
	img = cv2.imread('lightning.png')
	imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

	rows, cols = img.shape[:2]

	ret, thresh = cv2.threshold(imgray, 127,255,0)
	something, contour, hierarchy = cv2.findContours(thresh,1,2)

	cnt = contour[4]

	(x,y), r = cv2.minEnclosingCircle(cnt)
	center = (int(x),int(y))
	r = int(r)

	cv2.circle(img, center, r , (255,0,0),3)

	ellipse = cv2.fitEllipse(cnt)
	cv2.ellipse(img, ellipse, (0,255,0),3)

	[vx,vy,x,y] = cv2.fitLine(cnt,cv2.DIST_L2,0,0.01,0.01)
	ly = int((-x*vy/vx)+y)
	ry = int(((cols-x)*vy/vx)+y)

	cv2.line(img,(cols-1,ry),(0,ly),(0,0,255),2)

	cv2.imshow('fittings',img)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

if __name__ == '__main__':
	# convex()
	# convex2()
	convex3()