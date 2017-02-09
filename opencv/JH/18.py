import cv2

def moment():
	img = cv2.imread('test.png')
	imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

	ret, thresh = cv2.threshold(imgray, 127,255,0)
	something, contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

	contour = contours[0]
	momen = cv2.moments(contour)

	keys = momen.keys()

	for key in keys:
		print '%s : \t %f' %(key,momen[key])

	cx = int(momen['m10']/momen['m00'])
	cy = int(momen['m01']/momen['m00'])

	print cx, cy

def moment2():
	img = cv2.imread('test.png')
	imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

	ret ,thresh = cv2.threshold(imgray, 127, 255, 0)
	something, contours, hierarchy = cv2.findContours(thresh, 1, 2)

	cnt = contours[5]

	area = cv2.contourArea(cnt)
	perimeter = cv2.arcLength(cnt, True)

	cv2.drawContours(img, [cnt],0,(0,255,0),1)

	print area
	print perimeter

	cv2.imshow('contour',img)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

def contour():
	img = cv2.imread('bbox.png')
	img1 = img.copy()
	img2 = img.copy()

	imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

	ret ,thresh = cv2.threshold(imgray, 127,255, 0)
	something, contours, hierarchy = cv2.findContours(thresh,1,2)

	cnt = contours[0]
	cv2.drawContours(img, [cnt], 0, (0,255,0),3)

	epsilon1 = 0.01*cv2.arcLength(cnt, True)
	epsilon2 = 0.1*cv2.arcLength(cnt, True)

	approx1 = cv2.approxPolyDP(cnt, epsilon1, True)
	approx2 = cv2.approxPolyDP(cnt, epsilon2, True)

	cv2.drawContours(img1, [approx1],0,(0,255,0),3)
	cv2.drawContours(img2, [approx2],0,(0,255,0),3)

	cv2.imshow('contour',img)
	cv2.imshow('Approx1',img1)
	cv2.imshow('Approx2',img2)

	cv2.waitKey(0)
	cv2.destroyAllWindows()

if __name__ == '__main__':
	# moment()
	# moment2()
	contour()