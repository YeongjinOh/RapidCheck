import numpy as np
import cv2

def contour():
	img = cv2.imread('start1.jpg')
	imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
	ret, thr = cv2.threshold(imgray,127,255,0)

	something, contours, hierarchy = cv2.findContours(thr, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
	cnt = contours[0]
	print 'contour leng : ', len(contours)
	hull = cv2.convexHull(cnt)

	cv2.drawContours(img, [hull],0,(0,0,255),2)
	hull = cv2.convexHull(cnt, returnPoints = False)
	defects = cv2.convexityDefects(cnt, hull)

	for i in xrange(defects.shape[0]):
		sp, ep, fp, dist = defects[i,0]
		start = tuple(cnt[sp][0])
		end = tuple(cnt[ep][0])
		farthest = tuple(cnt[fp][0])

		cv2.circle(img, farthest,5,(0,255,0),-1)

	cv2.imshow('defects',img)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

def contour2():
	img = cv2.imread('start1.jpg')
	imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
	ret, thr = cv2.threshold(imgray, 127,255,0)

	something, contours, hierarchy = cv2.findContours(thr,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
	cnt = contours[0]
	cv2.drawContours(img, [cnt],0,(0,255,0),2)

	outside = (55,70)
	inside = (140,150)

	dist1 = cv2.pointPolygonTest(cnt, outside, True)
	dist2 = cv2.pointPolygonTest(cnt, inside, True)

	print 'shortest distance from contour to (%d, %d): %f ' %(outside[0],outside[1],dist1)
	print 'shortest distance from contour to (%d, %d): %f ' %(inside[0],inside[1],dist2)

	cv2.circle(img,outside, 3, (0,255,255),-1)
	cv2.circle(img,inside, 3, (255,0,255),-1)

	cv2.imshow('contour', img)
	cv2.waitKey(0)
	cv2.destroyAllWindows()


if __name__ == '__main__':
	contour()
	contour2()